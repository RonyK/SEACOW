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

	template <typename Ty_>
	struct SpihtNode
	{
	public:
		Ty_ value_;
		Ty_ D_;
		Ty_ G_;
		bool firstBand_ = false;
	};

//////////////////////////////
// Serializable
//////////////////////////////
public:
	void encode_init(dimension& bandDims);
	void decode_init(dimension& bandDims);
	
	template<typename Ty_>
	std::vector<Ty_> checkDescendants(std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, coor parent_coor, Ty_ signBit, size_t curLevel)
	{
		std::vector<Ty_> parentDG(2, 0);	// for parent
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;
		curLevel++;

		coor child_coor(parent_coor);
		if (curLevel == 1)		// first band
		{
			child_coor /= 2;
			child_coor *= 2;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				if (parent_coor[d] & 0x1)
				{
					child_coor[d] = child_coor[d] + bandDims[d];
				}
			}
		} else
		{
			child_coor *= 2;
		}
		
		for (int i = 0; i < (int)pow(2, dSize); i++)
		{
			itemItr->moveTo(child_coor);
			auto value = (**itemItr).get<Ty_>();

			// absolute 
			if ((value & signBit) && (value != signBit))
			{
				value = ~value;
				value += 1;
				value = value ^ signBit;
			}

			// check child and set index
			size_t index = 0;
			size_t multiply = 1;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				index += multiply * child_coor[d];
				multiply *= blockDims[d];
			}
			arr[index].value_ = value;
			(**itemItr).set<Ty_>(value);

			if (curLevel == this->maxLevel_)
			{
				parentDG[0] = parentDG[0] | value;
			}
			else
			{
				std::vector<Ty_> childDG = this->checkDescendants(arr, bandDims, child_coor, signBit, curLevel);		// for me
				arr[index].D_ = childDG[0];
				arr[index].G_ = childDG[1];

				parentDG[0] = parentDG[0] | arr[index].D_ | value;
				parentDG[1] = parentDG[1] | arr[index].D_;
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				child_coor[d] = child_coor[d] + 1;
				if (child_coor[d] & 0x1)
				{
					break;
				}
				else
				{
					child_coor[d] = child_coor[d] - 2;
				}
			}	
		}

		return parentDG;
	}

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		bs << setw(1);
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_);

		this->encode_init(bandDims);
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

		size_t block_num = 1;
		size_t band_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			block_num *= blockDims[d];
			band_num *= bandDims[d];
		}
		std::vector<SpihtNode<Ty_>> arr(block_num);

		// descendants travel 
		coor band_coor(dSize);
		for (size_t i = 0; i < band_num; i++)
		{
			itemItr->moveTo(band_coor);
			auto value = (**itemItr).get<Ty_>();

			// absolute 
			if ((value & signBit) && (value != signBit))
			{
				value = ~value;
				value += 1;
				value = value ^ signBit;
			}

			// check child and set index
			size_t index = 0;
			size_t multiply = 1;
			bool haveChild = false;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				index += multiply * band_coor[d];
				multiply *= blockDims[d];
				if (band_coor[d] & 0x1)		// odd coordinate
				{
					haveChild = true;
				}
			}
			(**itemItr).set<Ty_>(value);
			arr[index].value_ = value;
			arr[index].firstBand_ = true;

			// descendants travel
			if (haveChild)
			{
				std::vector<Ty_> DG = this->checkDescendants<Ty_>(arr, bandDims, band_coor, signBit, 0);
				arr[index].D_ = DG[0];
				arr[index].G_ = DG[1];
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				band_coor[d] = band_coor[d] + 1;
				if (band_coor[d] == bandDims[d])
				{
					band_coor[d] = 0;
				}
				else
				{
					break;
				}
			}	
		}

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->ENLSP_.size();
			this->encode_sigpass<Ty_>(bs, arr, bandDims, signBit, stepBit);
			this->encode_refinepass<Ty_>(bs, arr, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}

		// for -128 and 0
		size_t LIP_size = this->ENLIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			size_t LIP_index = this->ENLIP_.front();
			this->ENLIP_.pop_front();
			Ty_ LIP_value = arr[LIP_index].value_;

			if (LIP_value == 0)
			{
				bs << 0;
			} else if(LIP_value == signBit)
			{
				bs << 1;
			}

		}

		size_t LIS_size = this->ENLIS_.size();
		for (size_t i = 0; i < LIS_size; i++)
		{
			coor LIS_coor = this->ENLIS_.front();
			this->ENLIS_.pop_front();
			itemItr->moveTo(LIS_coor);
			Ty_ LIS_value = (**itemItr).get<Ty_>();

			if (LIS_value == 0)
			{
				bs << 0;
			}
			else if (LIS_value == signBit)
			{
				bs << 1;
			}

		}
	}

	template<class Ty_>
	void encode_sigpass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, Ty_ signBit, Ty_ stepBit)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		// LIP
		size_t LIP_size = this->ENLIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			size_t LIP_index = this->ENLIP_.front();
			this->ENLIP_.pop_front();
			Ty_ LIP_value = arr[LIP_index].value_;

			if (LIP_value & stepBit)
			{
				bs << 1;
				if (LIP_value & signBit)  // -
				{
					bs << 1;
				} else  // +
				{
					bs << 0;
				}
				this->ENLSP_.push_back(LIP_index);
			} else
			{
				bs << 0;
				this->ENLIP_.push_back(LIP_index);
			}
		}

		// LIS
		std::list<coor> TMP_;
		std::list<char> TMP_TYPE_;
		while (this->ENLIS_.size() != 0)
		{
			coor LIS_coor = this->ENLIS_.front();
			this->ENLIS_.pop_front();

			size_t LIS_index = 0;
			size_t multiply = 1;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				LIS_index += multiply * LIS_coor[d];
				multiply *= blockDims[d];
			}
			Ty_ LIS_value = arr[LIS_index].value_;

			// set child_coor and index
			coor child_coor(LIS_coor);
			size_t child_index = 0;
			if (arr[LIS_index].firstBand_)
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

				multiply = 1;
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					child_index += multiply * child_coor[d];
					multiply *= blockDims[d];
				}

			} else    // other level bands
			{
				child_coor = LIS_coor * 2;
				child_index = LIS_index * 2;
			}

			char LIS_type = this->ENLIS_TYPE_.front();
			this->ENLIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				if (arr[LIS_index].D_ & stepBit)
				{
					bs << 1;

					bool haveGrand = true;
					for (int d = (int)dSize - 1; d >= 0; d--)
					{
						if (child_coor[d] * 2 >= blockDims[d])
						{
							haveGrand = false;
							break;
						}
					}

					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						Ty_ child_value = arr[child_index].value_;

						if (child_value & stepBit)
						{
							bs << 1;
							if (child_value & signBit)  // -
							{
								bs << 1;
							} else  // +
							{
								bs << 0;
							}
							this->ENLSP_.push_back(child_index);
						} else
						{
							bs << 0;
							this->ENLIP_.push_back(child_index);
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

						child_index = 0;
						multiply = 1;
						for (int d = (int)dSize - 1; d >= 0; d--)
						{
							child_index += multiply * child_coor[d];
							multiply *= blockDims[d];
						}
					}

					if (haveGrand)
					{
						this->ENLIS_.push_back(LIS_coor);
						this->ENLIS_TYPE_.push_back('B');
					}
				} else
				{
					bs << 0;
					TMP_.push_back(LIS_coor);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				if (arr[LIS_index].G_ & stepBit)
				{
					bs << 1;

					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
					{
						this->ENLIS_.push_back(child_coor);
						this->ENLIS_TYPE_.push_back('A');

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
		this->ENLIS_ = TMP_;
		this->ENLIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void encode_refinepass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, Ty_ stepBit, size_t LSP_size)
	{
		std::list<size_t> TMP_ = this->ENLSP_;
		for (size_t i = 0; i < LSP_size; i++)
		{
			size_t LSP_index = TMP_.front();
			TMP_.pop_front();
			Ty_ LSP_value = arr[LSP_index].value_;

			if (LSP_value & stepBit)
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

		this->decode_init(bandDims);
		this->decode_progress<Ty_>(bs, bandDims);
	}

	template<class Ty_>
	void decode_progress(bstream& bs, dimension& bandDims)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		// set zero
		//coor zero_coor(dSize);	// {0, 0, ...}
		//size_t zero_num = 1;
		//for (int d = (int)dSize - 1; d >= 0; d--)
		//{
		//	zero_coor[d] = 0;
		//	zero_num *= blockDims[d];
		//}

		//for (size_t i = 0; i < zero_num; i++)
		//{
		//	itemItr->moveTo(zero_coor);
		//	Ty_ v = 0;
		//	v = (**itemItr).get<Ty_>();
		//	if(v != 0)
		//	{
		//		v = 0;
		//	}
		//	//(**itemItr).set<Ty_>(0);
		//	

		//	for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		//	{
		//		zero_coor[d] = zero_coor[d] + 1;
		//		if (zero_coor[d] == blockDims[d])
		//		{
		//			zero_coor[d] = 0;
		//		}
		//		else
		//		{
		//			break;
		//		}
		//	}
		//}
		
		size_t maxStep = sizeof(Ty_) * 8;
		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->DELSP_.size();
			this->decode_sigpass<Ty_>(bs, bandDims, signBit, stepBit);
			this->decode_refinepass<Ty_>(bs, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}

		// abs 
		// TODO:: unsigned error
		//coor abs_coor(dSize);	// {0, 0, ...}
		//size_t abs_num = 1;
		//for (int d = (int)dSize - 1; d >= 0; d--)
		//{
		//	abs_coor[d] = 0;
		//	abs_num *= blockDims[d];
		//}

		//for (size_t i = 0; i < abs_num; i++)
		//{
		//	itemItr->moveTo(abs_coor);
		//	auto data = (**itemItr).get<Ty_>();
		//	if ((data & signBit) && (data != signBit))
		//	{
		//		data = ~data;
		//		data += 1;
		//		data = data ^ signBit;
		//		(**itemItr).set<Ty_>(data);
		//	}

		//	for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		//	{
		//		abs_coor[d] = abs_coor[d] + 1;
		//		if (abs_coor[d] == blockDims[d])
		//		{
		//			abs_coor[d] = 0;
		//		} else
		//		{
		//			break;
		//		}
		//	}
		//}
		
		itemItr->moveToStart();
		while (!itemItr->isEnd())
		{
			auto data = (**itemItr).get<Ty_>();
			if (data & signBit)
			{
				data = ~data;
				data += 1;
				data = data ^ signBit;
				(**itemItr).set<Ty_>(data);
			}
			++(*itemItr);
		}

		// for -128 and 0
		char codeBit;
		size_t LIP_size = this->DELIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			coor LIP_coor = this->DELIP_.front();
			this->DELIP_.pop_front();
			itemItr->moveTo(LIP_coor);
			Ty_ LIP_value = (**itemItr).get<Ty_>();

			if (LIP_value == 0)
			{
				bs >> codeBit;
				if (codeBit)
				{
					LIP_value = LIP_value ^ signBit;
					(**itemItr).set<Ty_>(LIP_value);
				}
			}
		}

		size_t LIS_size = this->DELIS_.size();
		for (size_t i = 0; i < LIS_size; i++)
		{
			coor LIS_coor = this->DELIS_.front();
			this->DELIS_.pop_front();
			itemItr->moveTo(LIS_coor);
			Ty_ LIS_value = (**itemItr).get<Ty_>();

			if (LIS_value == 0)
			{
				bs >> codeBit;
				if (codeBit)
				{
					LIS_value = LIS_value ^ signBit;
					(**itemItr).set<Ty_>(LIS_value);
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
		size_t LIP_size = this->DELIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			auto LIP_coor = this->DELIP_.front();
			this->DELIP_.pop_front();
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
				this->DELSP_.push_back(LIP_coor);
			} else
			{
				this->DELIP_.push_back(LIP_coor);
			}
		}

		// LIS
		std::list<coor> TMP_;
		std::list<char> TMP_TYPE_;
		coor grand_coor(dSize);
		while (this->DELIS_.size() != 0)
		{
			coor LIS_coor = this->DELIS_.front();
			this->DELIS_.pop_front();
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

			char LIS_type = this->DELIS_TYPE_.front();
			this->DELIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				bs >> codeBit;

				if (codeBit)
				{
					bool haveGrand = true;
					for (int d = (int)dSize - 1; d >= 0; d--)
					{
						if (child_coor[d] * 2 >= blockDims[d])
						{
							haveGrand = false;
							break;
						}
					}

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
							this->DELSP_.push_back(child_coor);
						} else
						{
							this->DELIP_.push_back(child_coor);
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

					if (haveGrand)
					{
						this->DELIS_.push_back(LIS_coor);
						this->DELIS_TYPE_.push_back('B');
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
						this->DELIS_.push_back(child_coor);
						this->DELIS_TYPE_.push_back('A');

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
		this->DELIS_ = TMP_;
		this->DELIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void decode_refinepass(bstream& bs, Ty_ stepBit, size_t LSP_size)
	{
		auto itemItr = this->getItemIterator();
		char codeBit;

		std::list<coor> TMP_ = this->DELSP_;
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
	std::list<size_t> ENLIP_;
	std::list<coor> ENLIS_;
	std::list<char> ENLIS_TYPE_;
	std::list<size_t> ENLSP_;

	std::list<coor> DELIP_;
	std::list<coor> DELIS_;
	std::list<char> DELIS_TYPE_;
	std::list<coor> DELSP_;

	size_t maxLevel_;
};
}
#endif	// _MSDB_SPIHTBLOCK_H_
