#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_ACTION_H
#define _MSDB_OP_SPIHT_DECODE_ACTION_H

#include <query/opAction.h>
#include <util/math.h>
#include <list>

namespace msdb
{
class spiht_decode_action : public opAction
{
public:
	spiht_decode_action();
	virtual ~spiht_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& outputArrays, pQuery q);

//private:
//	void decode_init(size_t dSize, std::vector<size_t> bandSize);
//	template<class Ty_>
//	void decode_progress(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, pChunkItemIterator itemItr)
//	{
//		size_t maxStep = sizeof(Ty_) * 8;
//		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
//		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);
//
//		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
//		{
//			size_t LSP_size = this->LSP_.size();
//			this->decode_sigpass<Ty_>(dSize, cSize, bandSize, itemItr, signBit, stepBit);
//			this->decode_refinepass<Ty_>(itemItr, stepBit, LSP_size);
//			stepBit = stepBit >> 1;
//		}
//
//		// abs
//		coor init_coor(dSize);	// {0, 0, ...}
//		size_t abs_num = 1;
//		for (int d = (int)dSize - 1; d >= 0; d--)
//		{
//			init_coor[d] = 0;
//			abs_num *= cSize[d];
//		}
//
//		for (size_t i = 0; i < abs_num; i++)
//		{
//			itemItr->moveTo(init_coor);
//			auto data = (**itemItr).get<Ty_>();
//			if (data & signBit)
//			{
//				data = data * -1;
//				data = data ^ signBit;
//				(**itemItr).set<Ty_>(data);
//			}
//
//			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//			{
//				init_coor[d] = init_coor[d] + 1;
//				if (init_coor[d] == cSize[d])
//				{
//					init_coor[d] = 0;
//				} else
//				{
//					break;
//				}
//			}
//		}
//	}
//
//	template<class Ty_>
//	void decode_sigpass(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize,
//						pChunkItemIterator itemItr, Ty_ signBit, Ty_ stepBit)
//	{
//		int codeBit; // TODO
//
//		// LIP
//		size_t LIP_size = this->LIP_.size();
//		for (size_t i = 0; i < LIP_size; i++)
//		{
//			auto LIP_coor = this->LIP_.front();
//			this->LIP_.pop_front();
//			itemItr->moveTo(LIP_coor);
//			auto LIP_data = (**itemItr).get<Ty_>();
//
//			codeBit = this->code.front();	// TODO
//			this->code.pop_front();			// TODO
//
//			if (codeBit)
//			{
//				LIP_data = LIP_data ^ stepBit;
//
//				codeBit = this->code.front();	// TODO
//				this->code.pop_front();			// TODO
//
//				if (codeBit)  // -
//				{
//					LIP_data = LIP_data ^ signBit;
//				}
//
//				(**itemItr).set<Ty_>(LIP_data);
//				this->LSP_.push_back(LIP_coor);
//			} else
//			{
//				this->LIP_.push_back(LIP_coor);
//			}
//
//		}
//
//		// LIS
//		std::list<coor> TMP_;
//		std::list<char> TMP_TYPE_;
//		coor child_coor(dSize);
//		coor grand_coor(dSize);
//		while (this->LIS_.size() != 0)
//		{
//			coor LIS_coor = this->LIS_.front();
//			this->LIS_.pop_front();
//			itemItr->moveTo(LIS_coor);
//			auto LIS_data = (**itemItr).get<Ty_>();
//
//			// set child_coor
//			bool firstBand = true;
//			for (int d = (int)dSize - 1; d >= 0; d--)
//			{
//				if (LIS_coor[d] >= bandSize[d])
//				{
//					firstBand = false;
//					break;
//				}
//			}
//
//			if (firstBand)
//			{
//				for (int d = (int)dSize - 1; d >= 0; d--)
//				{
//					child_coor[d] = (LIS_coor[d] / 2) * 2;
//					if (LIS_coor[d] & 0x1)
//					{
//						child_coor[d] = child_coor[d] + bandSize[d];
//					}
//				}
//			} else    // other level bands
//			{
//				for (int d = (int)dSize - 1; d >= 0; d--)
//				{
//					child_coor[d] = LIS_coor[d] * 2;
//				}
//			}
//			coor temp_coor = child_coor;
//
//			char LIS_type = this->LIS_TYPE_.front();
//			this->LIS_TYPE_.pop_front();
//			if (LIS_type == 'A')	// type A
//			{
//				codeBit = this->code.front();	// TODO
//				this->code.pop_front();			// TODO
//
//				if (codeBit)
//				{
//					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
//					{
//						itemItr->moveTo(child_coor);
//						auto child_data = (**itemItr).get<Ty_>();
//
//						codeBit = this->code.front();	// TODO
//						this->code.pop_front();			// TODO
//
//						if (codeBit)
//						{
//							child_data = child_data ^ stepBit;
//
//							codeBit = this->code.front();	// TODO
//							this->code.pop_front();			// TODO
//
//							if (codeBit)  // -
//							{
//								child_data = child_data ^ signBit;
//							}
//
//							(**itemItr).set<Ty_>(child_data);
//							this->LSP_.push_back(child_coor);
//						} else
//						{
//							this->LIP_.push_back(child_coor);
//						}
//
//						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//						{
//							child_coor[d] = child_coor[d] + 1;
//							if (child_coor[d] & 0x1)
//							{
//								break;
//							} else
//							{
//								child_coor[d] = child_coor[d] - 2;
//							}
//						}
//					}
//
//					bool haveGrand = true;
//					child_coor = temp_coor;
//					for (int d = (int)dSize - 1; d >= 0; d--)
//					{
//						if (child_coor[d] * 2 >= cSize[d])
//						{
//							haveGrand = false;
//							break;
//						}
//					}
//
//					if (haveGrand)
//					{
//						this->LIS_.push_back(LIS_coor);
//						this->LIS_TYPE_.push_back('B');
//					}
//				} else
//				{
//					TMP_.push_back(LIS_coor);
//					TMP_TYPE_.push_back(LIS_type);
//				}
//			} else    // type B
//			{
//				codeBit = this->code.front();	// TODO
//				this->code.pop_front();			// TODO
//
//				if (codeBit)
//				{
//					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
//					{
//						this->LIS_.push_back(child_coor);
//						this->LIS_TYPE_.push_back('A');
//
//						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//						{
//							child_coor[d] = child_coor[d] + 1;
//							if (child_coor[d] & 0x1)
//							{
//								break;
//							} else
//							{
//								child_coor[d] = child_coor[d] - 2;
//							}
//						}
//					}
//				} else
//				{
//					TMP_.push_back(LIS_coor);
//					TMP_TYPE_.push_back(LIS_type);
//				}
//			}
//		}
//		this->LIS_ = TMP_;
//		this->LIS_TYPE_ = TMP_TYPE_;
//	}
//
//	template<class Ty_>
//	void decode_refinepass(pChunkItemIterator itemItr, Ty_ stepBit, size_t LSP_size)
//	{
//		for (size_t i = 0; i < LSP_size; i++)
//		{
//			coor LSP_coor = this->LSP_.front();
//			this->LSP_.pop_front();
//			itemItr->moveTo(LSP_coor);
//			auto LSP_data = (**itemItr).get<Ty_>();
//
//			auto codeBit = this->code.front();	// TODO
//			this->code.pop_front();			// TODO
//
//			if (codeBit)
//			{
//				LSP_data = LSP_data ^ stepBit;
//				(**itemItr).set<Ty_>(LSP_data);
//			}
//		}
//	}
//
//private:
//	std::list<coor> LIP_;
//	std::list<coor> LIS_;
//	std::list<char> LIS_TYPE_;
//	std::list<coor> LSP_;
//
//	bstream codeBs_;
};
}


#endif // _MSDB_OP_SPIHT_DECODE_ACTION_H
