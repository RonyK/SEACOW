#pragma once
#ifndef _MSDB_SPIHTBLOCK_H_
#define _MSDB_SPIHTBLOCK_H_

#include <array/memBlock.h>
#include <io/bitstream.h>
#include <list>

namespace msdb
{
class spihtBlock;
using pSpihtBlock = std::shared_ptr<spihtBlock>;

class spihtBlock : public memBlock
{
public:
	spihtBlock(pBlockDesc desc);
	virtual ~spihtBlock();

//////////////////////////////
// Serializable
//////////////////////////////
public:
	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_ + 1);

		this->encode_init(bandDims);
		this->encode_progress<Ty_>(bs, bandDims);
	}

	void encode_init(dimension& bandDims);

	template<class Ty_>
	void encode_progress(bstream& bs, dimension& bandDims)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		size_t maxStep = sizeof(Ty_) * 8;
		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);
		// abs
		coor init_coor(dSize);	// {0, 0, ...}
		size_t abs_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			init_coor[d] = 0;
			abs_num *= blockDims[d];
		}

		for (size_t i = 0; i < abs_num; i++)
		{
			itemItr->moveTo(init_coor);
			auto data = (**itemItr).get<Ty_>();
			if (data & signBit)
			{
				data = data * -1;
				data = data ^ signBit;
				(**itemItr).set<Ty_>(data);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				init_coor[d] = init_coor[d] + 1;
				if (init_coor[d] == blockDims[d])
				{
					init_coor[d] = 0;
				} else
				{
					break;
				}
			}
		}

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->LSP_.size();
			this->encode_sigpass<Ty_>(bs, dSize, blockDims, bandDims, itemItr, signBit, stepBit);
			this->encode_refinepass<Ty_>(bs, itemItr, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}
	}

	template<class Ty_>
	void encode_sigpass(bstream& bs, size_t dSize, dimension& blockDims, dimension& bandDims,
						pBlockItemIterator itemItr, Ty_ signBit, Ty_ stepBit)
	{
		// LIP
		size_t LIP_size = this->LIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			auto LIP_coor = this->LIP_.front();
			this->LIP_.pop_front();
			itemItr->moveTo(LIP_coor);
			auto LIP_data = (**itemItr).get<Ty_>();

			if (LIP_data & stepBit)
			{
				//this->code.push_back(1);	// TODO
				bs << 1;
				if (LIP_data & signBit)  // -
				{
					//this->code.push_back(1);	// TODO
					bs << 1;
				} else  // +
				{
					//this->code.push_back(0);	// TODO
					bs << 0;
				}
				this->LSP_.push_back(LIP_coor);
			} else
			{
				//this->code.push_back(0);	// TODO
				bs << 0;
				this->LIP_.push_back(LIP_coor);
			}
		}

		// LIS
		std::list<coor> TMP_;
		std::list<char> TMP_TYPE_;
		coor child_coor(dSize);
		coor grand_coor(dSize);
		while (this->LIS_.size() != 0)
		{
			coor LIS_coor = this->LIS_.front();
			this->LIS_.pop_front();
			itemItr->moveTo(LIS_coor);
			auto LIS_data = (**itemItr).get<Ty_>();

			// set child_coor
			bool firstBand = true;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				if (LIS_coor[d] >= bandDims[d])
				{
					firstBand = false;
					break;
				}
			}

			if (firstBand)
			{
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					child_coor[d] = (LIS_coor[d] / 2) * 2;
					if (LIS_coor[d] & 0x1)
					{
						child_coor[d] = child_coor[d] + bandDims[d];
					}
				}
			} else    // other level bands
			{
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					child_coor[d] = LIS_coor[d] * 2;
				}
			}
			coor temp_coor = child_coor;

			char LIS_type = this->LIS_TYPE_.front();
			this->LIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				bool oneFlag = false;
				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
				{
					itemItr->moveTo(child_coor);
					auto child_data = (**itemItr).get<Ty_>();

					if (child_data & stepBit)
					{
						oneFlag = true;
						break;
					}

					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
					{
						child_coor[d] = child_coor[d] + 1;
						if (child_coor[d] & 0x1)
						{
							break;
						} else
						{
							child_coor[d] = child_coor[d] - 2;
						}
					}
				}

				if (oneFlag)
				{
					// this->code.push_back(1);	// TODO
					bs << 1;

					child_coor = temp_coor;
					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						itemItr->moveTo(child_coor);
						auto child_data = (**itemItr).get<Ty_>();

						if (child_data & stepBit)
						{
							// this->code.push_back(1);	// TODO
							bs << 1;
							if (child_data & signBit)  // -
							{
								// this->code.push_back(1);	// TODO
								bs << 1;
							} else  // +
							{
								// this->code.push_back(0);	// TODO
								bs << 0;
							}
							this->LSP_.push_back(child_coor);
						} else
						{
							// this->code.push_back(0);	// TODO
							bs << 0;
							this->LIP_.push_back(child_coor);
						}

						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
						{
							child_coor[d] = child_coor[d] + 1;
							if (child_coor[d] & 0x1)
							{
								break;
							} else
							{
								child_coor[d] = child_coor[d] - 2;
							}
						}
					}

					bool haveGrand = true;
					child_coor = temp_coor;
					for (int d = (int)dSize - 1; d >= 0; d--)
					{
						if (child_coor[d] * 2 >= blockDims[d])
						{
							haveGrand = false;
							break;
						}
					}

					if (haveGrand)
					{
						this->LIS_.push_back(LIS_coor);
						this->LIS_TYPE_.push_back('B');
					}
				} else
				{
					// this->code.push_back(0);	// TODO
					bs << 0x0;
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				child_coor = temp_coor;
				bool oneFlag = false;
				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
				{
					for (int d = (int)dSize - 1; d >= 0; d--)
					{
						grand_coor[d] = child_coor[d] * 2;
					}

					for (size_t j = 0; j < (size_t)pow(2, dSize); j++)
					{
						itemItr->moveTo(grand_coor);
						auto grand_data = (**itemItr).get<Ty_>();

						if (grand_data & stepBit)
						{
							oneFlag = true;
							break;
						}

						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
						{
							grand_coor[d] = grand_coor[d] + 1;
							if (grand_coor[d] & 0x1)
							{
								break;
							} else
							{
								grand_coor[d] = grand_coor[d] - 2;
							}
						}
					}

					if (oneFlag)
					{
						break;
					}

					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
					{
						child_coor[d] = child_coor[d] + 1;
						if (child_coor[d] & 0x1)
						{
							break;
						} else
						{
							child_coor[d] = child_coor[d] - 2;
						}
					}
				}

				if (oneFlag)
				{
					// this->code.push_back(1);	// TODO
					bs << 0x1;

					child_coor = temp_coor;
					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						this->LIS_.push_back(child_coor);
						this->LIS_TYPE_.push_back('A');

						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
						{
							child_coor[d] = child_coor[d] + 1;
							if (child_coor[d] & 0x1)
							{
								break;
							} else
							{
								child_coor[d] = child_coor[d] - 2;
							}
						}
					}
				} else
				{
					// this->code.push_back(0);	// TODO
					bs << 0x0;
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			}
		}
		this->LIS_ = TMP_;
		this->LIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void encode_refinepass(bstream& bs, pBlockItemIterator itemItr, Ty_ stepBit, size_t LSP_size)
	{
		for (size_t i = 0; i < LSP_size; i++)
		{
			coor LSP_coor = this->LSP_.front();
			this->LSP_.pop_front();
			itemItr->moveTo(LSP_coor);
			auto LSP_data = (**itemItr).get<Ty_>();

			if (LSP_data & stepBit)
			{
				// this->code.push_back(1);	// TODO
				bs << 0x1;
			} else
			{
				// this->code.push_back(0);	// TODO
				bs << 0x0;
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		bs >> setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();
		while (!it->isEnd())
		{
			Ty_ value;
			bs >> value;
			(**it).set<Ty_>(value);
			++(*it);
		}
	}

public:
	void setMaxLevel(size_t maxLevel);

private:
	std::list<coor> LIP_;
	std::list<coor> LIS_;
	std::list<char> LIS_TYPE_;
	std::list<coor> LSP_;

	size_t maxLevel_;
};
}
#endif	// _MSDB_SPIHTBLOCK_H_
