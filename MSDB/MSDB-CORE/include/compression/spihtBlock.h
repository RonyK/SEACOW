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
	void init(dimension& bandDims);

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		bs << setw(1);
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_);

		this->init(bandDims);
		this->encode_progress<Ty_>(bs, bandDims);
	}

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
		coor abs_coor(dSize);	// {0, 0, ...}
		size_t abs_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			abs_coor[d] = 0;
			abs_num *= blockDims[d];
		}

		for (size_t i = 0; i < abs_num; i++)
		{
			itemItr->moveTo(abs_coor);
			auto data = (**itemItr).get<Ty_>();
			if (data & signBit)
			{
				data = data * -1;
				data = data ^ signBit;
				(**itemItr).set<Ty_>(data);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				abs_coor[d] = abs_coor[d] + 1;
				if (abs_coor[d] == blockDims[d])
				{
					abs_coor[d] = 0;
				} else
				{
					break;
				}
			}
		}

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->LSP_.size();
			this->encode_sigpass<Ty_>(bs, bandDims, signBit, stepBit);
			this->encode_refinepass<Ty_>(bs, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}
	}

	template<class Ty_>
	void encode_sigpass(bstream& bs, dimension& bandDims, Ty_ signBit, Ty_ stepBit)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

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
				bs << 1;
				if (LIP_data & signBit)  // -
				{
					bs << 1;
				} else  // +
				{
					bs << 0;
				}
				this->LSP_.push_back(LIP_coor);
			} else
			{
				bs << 0;
				this->LIP_.push_back(LIP_coor);
			}
		}

		// LIS
		std::list<coor> TMP_;
		std::list<char> TMP_TYPE_;
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

			coor child_coor(LIS_coor);
			if (firstBand)
			{
				child_coor /= 2;
				child_coor *= 2;
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					if (LIS_coor[d] & 0x1)
					{
						child_coor[d] = child_coor[d] + bandDims[d];
					}
				}
			} else    // other level bands
			{
				child_coor *= 2;
			}
			coor tmp_coor = child_coor;

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
					bs << 1;

					child_coor = tmp_coor;
					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						itemItr->moveTo(child_coor);
						auto child_data = (**itemItr).get<Ty_>();

						if (child_data & stepBit)
						{
							bs << 1;
							if (child_data & signBit)  // -
							{
								bs << 1;
							} else  // +
							{
								bs << 0;
							}
							this->LSP_.push_back(child_coor);
						} else
						{
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
					child_coor = tmp_coor;
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
					bs << 0;
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				child_coor = tmp_coor;
				bool oneFlag = false;
				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
				{
					coor grand_coor(child_coor);
					grand_coor *= 2;

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
					bs << 1;

					child_coor = tmp_coor;
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
					bs << 0;
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			}
		}
		this->LIS_ = TMP_;
		this->LIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void encode_refinepass(bstream& bs, Ty_ stepBit, size_t LSP_size)
	{
		auto itemItr = this->getItemIterator();

		std::list<coor> TMP_ = this->LSP_;
		for (size_t i = 0; i < LSP_size; i++)
		{
			coor LSP_coor = TMP_.front();
			TMP_.pop_front();
			itemItr->moveTo(LSP_coor);
			auto LSP_data = (**itemItr).get<Ty_>();

			if (LSP_data & stepBit)
			{
				bs << 1;
			} else
			{
				bs << 0;
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		bs >> setw(1);
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_);

		this->init(bandDims);
		this->decode_progress<Ty_>(bs, bandDims);
	}

	template<class Ty_>
	void decode_progress(bstream& bs, dimension& bandDims)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		// set zero
		coor zero_coor(dSize);	// {0, 0, ...}
		size_t zero_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			zero_coor[d] = 0;
			zero_num *= blockDims[d];
		}

		for (size_t i = 0; i < zero_num; i++)
		{
			itemItr->moveTo(zero_coor);
			(**itemItr).set<Ty_>(0);
			

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				zero_coor[d] = zero_coor[d] + 1;
				if (zero_coor[d] == blockDims[d])
				{
					zero_coor[d] = 0;
				}
				else
				{
					break;
				}
			}
		}
		
		size_t maxStep = sizeof(Ty_) * 8;
		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->LSP_.size();
			this->decode_sigpass<Ty_>(bs, bandDims, signBit, stepBit);
			this->decode_refinepass<Ty_>(bs, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}

		// abs 
		// TODO:: unsigned error
		coor abs_coor(dSize);	// {0, 0, ...}
		size_t abs_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			abs_coor[d] = 0;
			abs_num *= blockDims[d];
		}

		for (size_t i = 0; i < abs_num; i++)
		{
			itemItr->moveTo(abs_coor);
			auto data = (**itemItr).get<Ty_>();
			if (data & signBit)
			{
				data = data * -1;
				data = data ^ signBit;
				(**itemItr).set<Ty_>(data);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				abs_coor[d] = abs_coor[d] + 1;
				if (abs_coor[d] == blockDims[d])
				{
					abs_coor[d] = 0;
				} else
				{
					break;
				}
			}
		}
	}

	template<class Ty_>
	void decode_sigpass(bstream& bs, dimension& bandDims, Ty_ signBit, Ty_ stepBit)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;
		char codeBit;

		// LIP
		size_t LIP_size = this->LIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			auto LIP_coor = this->LIP_.front();
			this->LIP_.pop_front();
			itemItr->moveTo(LIP_coor);
			auto LIP_data = (**itemItr).get<Ty_>();

			bs >> codeBit;

			if (codeBit)
			{
				LIP_data = LIP_data ^ stepBit;

				bs >> codeBit;

				if (codeBit)  // -
				{
					LIP_data = LIP_data ^ signBit;
				}

				(**itemItr).set<Ty_>(LIP_data);
				this->LSP_.push_back(LIP_coor);
			} else
			{
				this->LIP_.push_back(LIP_coor);
			}
		}

		// LIS
		std::list<coor> TMP_;
		std::list<char> TMP_TYPE_;
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

			coor child_coor(LIS_coor);
			if (firstBand)
			{
				child_coor /= 2;
				child_coor *= 2;
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					if (LIS_coor[d] & 0x1)
					{
						child_coor[d] = child_coor[d] + bandDims[d];
					}
				}
			} else    // other level bands
			{
				child_coor *= 2;
			}
			coor tmp_coor = child_coor;

			char LIS_type = this->LIS_TYPE_.front();
			this->LIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				bs >> codeBit;

				if (codeBit)
				{
					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						itemItr->moveTo(child_coor);
						auto child_data = (**itemItr).get<Ty_>();

						bs >> codeBit;

						if (codeBit)
						{
							child_data = child_data ^ stepBit;

							bs >> codeBit;

							if (codeBit)  // -
							{
								child_data = child_data ^ signBit;
							}

							(**itemItr).set<Ty_>(child_data);
							this->LSP_.push_back(child_coor);
						} else
						{
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
					child_coor = tmp_coor;
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
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				bs >> codeBit;

				if (codeBit)
				{
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
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			}
		}
		this->LIS_ = TMP_;
		this->LIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void decode_refinepass(bstream& bs, Ty_ stepBit, size_t LSP_size)
	{
		auto itemItr = this->getItemIterator();
		char codeBit;

		std::list<coor> TMP_ = this->LSP_;
		for (size_t i = 0; i < LSP_size; i++)
		{
			coor LSP_coor = TMP_.front();
			TMP_.pop_front();
			itemItr->moveTo(LSP_coor);
			auto LSP_data = (**itemItr).get<Ty_>();

			bs >> codeBit;

			if (codeBit)
			{
				LSP_data = LSP_data ^ stepBit;
				(**itemItr).set<Ty_>(LSP_data);
			}
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
