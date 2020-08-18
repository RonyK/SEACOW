#pragma once
#ifndef _MSDB_OP_SPIHT_ENCODE_ACTION_H_
#define _MSDB_OP_SPIHT_ENCODE_ACTION_H_

#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
class spiht_encode_action : public opAction
{
public:
	spiht_encode_action();
	virtual ~spiht_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

//private:
//	void encode_init(size_t dSize, std::vector<size_t> bandSize);
//	template<class Ty_>
//	void encode_progress(size_t dSize, dimension blockDims, std::vector<size_t> bandSize, pBlockItemIterator itemItr)
//	{
//		size_t maxStep = sizeof(Ty_) * 8;
//		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
//		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);
//		// abs
//		coor init_coor(dSize);	// {0, 0, ...}
//		size_t abs_num = 1;
//		for (int d = (int)dSize - 1; d >= 0; d--)
//		{
//			init_coor[d] = 0;
//			abs_num *= blockDims[d];
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
//				if (init_coor[d] == blockDims[d])
//				{
//					init_coor[d] = 0;
//				}
//				else
//				{
//					break;
//				}
//			}
//		}
//
//		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
//		{
//			size_t LSP_size = this->LSP_.size();
//			this->encode_sigpass<Ty_>(dSize, blockDims, bandSize, itemItr, signBit, stepBit);
//			this->encode_refinepass<Ty_>(itemItr, stepBit, LSP_size);
//			stepBit = stepBit >> 1;
//		}
//	}
//
//	template<class Ty_>
//	void encode_sigpass(size_t dSize, dimension blockDims, std::vector<size_t> bandSize,
//		pBlockItemIterator itemItr, Ty_ signBit, Ty_ stepBit)
//	{
//		// LIP
//		size_t LIP_size = this->LIP_.size();
//		for (size_t i = 0; i < LIP_size; i++)
//		{
//			auto LIP_coor = this->LIP_.front();
//			this->LIP_.pop_front();
//			itemItr->moveTo(LIP_coor);
//			auto LIP_data = (**itemItr).get<Ty_>();
//
//			if (LIP_data & stepBit)
//			{
//				//this->code.push_back(1);	// TODO
//				this->codeBs_ << 1;
//				if (LIP_data & signBit)  // -
//				{
//					//this->code.push_back(1);	// TODO
//					this->codeBs_ << 1;
//				}
//				else  // +
//				{
//					//this->code.push_back(0);	// TODO
//					this->codeBs_ << 0;
//				}
//				this->LSP_.push_back(LIP_coor);
//			}
//			else
//			{
//				//this->code.push_back(0);	// TODO
//				this->codeBs_ << 0;
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
//			}
//			else    // other level bands
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
//				bool oneFlag = false;
//				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
//				{
//					itemItr->moveTo(child_coor);
//					auto child_data = (**itemItr).get<Ty_>();
//
//					if (child_data & stepBit)
//					{
//						oneFlag = true;
//						break;
//					}
//
//					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//					{
//						child_coor[d] = child_coor[d] + 1;
//						if (child_coor[d] & 0x1)
//						{
//							break;
//						}
//						else
//						{
//							child_coor[d] = child_coor[d] - 2;
//						}
//					}
//				}
//
//				if (oneFlag)
//				{
//					// this->code.push_back(1);	// TODO
//					this->codeBs_ << 1;
//
//					child_coor = temp_coor;
//					for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
//					{
//						itemItr->moveTo(child_coor);
//						auto child_data = (**itemItr).get<Ty_>();
//
//						if (child_data & stepBit)
//						{
//							// this->code.push_back(1);	// TODO
//							this->codeBs_ << 1;
//							if (child_data & signBit)  // -
//							{
//								// this->code.push_back(1);	// TODO
//								this->codeBs_ << 1;
//							}
//							else  // +
//							{
//								// this->code.push_back(0);	// TODO
//								this->codeBs_ << 0;
//							}
//							this->LSP_.push_back(child_coor);
//						}
//						else
//						{
//							// this->code.push_back(0);	// TODO
//							this->codeBs_ << 0;
//							this->LIP_.push_back(child_coor);
//						}
//
//						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//						{
//							child_coor[d] = child_coor[d] + 1;
//							if (child_coor[d] & 0x1)
//							{
//								break;
//							}
//							else
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
//						if (child_coor[d] * 2 >= blockDims[d])
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
//				}
//				else
//				{
//					// this->code.push_back(0);	// TODO
//					this->codeBs_ << 0;
//					TMP_.push_back(LIS_coor);
//					TMP_TYPE_.push_back(LIS_type);
//				}
//			}
//			else    // type B
//			{
//				child_coor = temp_coor;
//				bool oneFlag = false;
//				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
//				{
//					for (int d = (int)dSize - 1; d >= 0; d--)
//					{
//						grand_coor[d] = child_coor[d] * 2;
//					}
//
//					for (size_t j = 0; j < (size_t)pow(2, dSize); j++)
//					{
//						itemItr->moveTo(grand_coor);
//						auto grand_data = (**itemItr).get<Ty_>();
//
//						if (grand_data & stepBit)
//						{
//							oneFlag = true;
//							break;
//						}
//
//						for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//						{
//							grand_coor[d] = grand_coor[d] + 1;
//							if (grand_coor[d] & 0x1)
//							{
//								break;
//							}
//							else
//							{
//								grand_coor[d] = grand_coor[d] - 2;
//							}
//						}
//					}
//
//					if (oneFlag)
//					{
//						break;
//					}
//
//					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//					{
//						child_coor[d] = child_coor[d] + 1;
//						if (child_coor[d] & 0x1)
//						{
//							break;
//						}
//						else
//						{
//							child_coor[d] = child_coor[d] - 2;
//						}
//					}
//				}
//
//				if (oneFlag)
//				{
//					// this->code.push_back(1);	// TODO
//					this->codeBs_ << 1;
//
//					child_coor = temp_coor;
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
//							}
//							else
//							{
//								child_coor[d] = child_coor[d] - 2;
//							}
//						}
//					}
//				}
//				else
//				{
//					// this->code.push_back(0);	// TODO
//					this->codeBs_ << 0;
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
//	void encode_refinepass(pBlockItemIterator itemItr, Ty_ stepBit, size_t LSP_size)
//	{
//		for (size_t i = 0; i < LSP_size; i++)
//		{
//			coor LSP_coor = this->LSP_.front();
//			this->LSP_.pop_front();
//			itemItr->moveTo(LSP_coor);
//			auto LSP_data = (**itemItr).get<Ty_>();
//
//			if (LSP_data & stepBit)
//			{
//				// this->code.push_back(1);	// TODO
//				this->codeBs_ << 1;
//			}
//			else
//			{
//				// this->code.push_back(0);	// TODO
//				this->codeBs_ << 0;
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

#endif // _MSDB_OP_SPIHT_ENCODE_ACTION_H_