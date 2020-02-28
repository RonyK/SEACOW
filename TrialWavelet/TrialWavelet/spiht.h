#pragma once
#ifndef _caSPIHT_
#define _caSPIHT_

#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <bitset>
#include "wavelet.h"
#include "iterators.h"

// Bitset size
#define BS_SIZE_	64

namespace caWavelet
{
	template <typename Dty_, typename Ty_>
	class caSPIHT
	{
	public:
		caSPIHT()       
		{

		}

		int encode(Ty_& wtData, std::vector<std::bitset<BS_SIZE_>>& output, 
			std::vector<Dty_>& dims, size_t wtLevel)
		{
			this->data = &wtData;
			this->wtLevel_ = wtLevel;
			this->dims_ = &dims;

			enInit();

			for (this->step_ = 0; this->step_ < this->maxStep_; this->step_++)
			{
				enProgress(this->step_);
			}

			return 0;
		}
		int decode(std::vector<std::bitset<BS_SIZE_>>& spData, double& output)
		{
			// to do: 모든 coor의 값을 0으로 초기화 

			enInit();

			for (this->step_ = 0; this->step_ < this->maxStep_; this->step_++)
			{
				deProgress(this->step_);
			}

			return 0;
		}

	protected:
		void enInit()
		{
			LIS_.clear();
			LIP_.clear();
			LSP_.clear();

			// Insert
			caWTIterator<Dty_, Ty_> it(this->data, this->dims_.size(), this->dims_);
			it.setMaxLevel(this->wtLevel_);
			it.moveToStart();

			// Init LIP
			while (it.curBand() == 0 && it.curLevel() == this->wtLevel_)
			{
				this->LIP_.push_back(coorVal(it.coor(), *it));
				it++;
			}

			// Init LIS
			for (size_t band = 0; band < pow(2, this->dims_.size()); band++)
			{
				size_t dSize = this->dims_.size();
				caCoor<Dty_> coor(dSize);
				
				for (size_t d = this->dims_.size() - 1; d + 1 > 0; d++)
				{
					coor[d] = (band >> (dSize - d - 1)) & 0x01;
				}
				this->LIS_.push_back(coorValTy(coor, cType::typeA_));
			}
		}

		void enProgress(size_t step)
		{
			std::list<char> output;

			size_t LSP_size = LSP_.size();

			output = enSigPass(step);
			sig_output.push_back(output);

			output = enRefinePass(step, LSP_size);
			ref_output.push_back(output);
		}

		std::list<char> enSigPass(size_t k)
		{
			std::list<char> output;
			std::bitset<BS_SIZE_> bit_value;

			size_t LIP_size = LIP_.size();
			coorVal LIP_data;
			for (size_t i = 0; i < LIP_size; i++)
			{
				LIP_data = LIP_.front();
				LIP_.pop_front();

				bit_value = LIP_data.value_;

				if (bit_value[BS_SIZE_ - 2 - k] == 1)
				{
					output.push_back('1');

					if(LIP_data.value_ >= 0)
						output.push_back('+');
					else
						output.push_back('-');
				
					LSP_.push_back(LIP_data);
				}
				else
				{
					output.push_back('0');	
					LIP_.push_back(LIP_data);
				}
			}

			caWTIterator<Dty_, Ty_> child(this->data, this->dims_.size(), this->dims_);
			caWTIterator<Dty_, Ty_> grand(this->data, this->dims_.size(), this->dims_);
			std::list<coorValTy> TMP_LIS;
			coorValTy LIS_data;
			while (LIS_.size() != 0)
			{
				LIS_data = LIS_.front();
				LIS_.pop_front();

				if (LIS_data.type_ == cType::typeA_)
				{
					child.setCurCoor(LIS_data.coor_);
					child.moveToChild();

					int one_flag = 0;
					for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
					{
						bit_value = *child;

						if (bit_value[BS_SIZE_ - 2 - k] == 1)
						{
							one_flag = 1;
							break;
						}

						if (i != (pow(2, this->dims_.size()) - 1))
							child.moveToNextSibling();
					}

					if (one_flag == 1)
					{
						output.push_back('1');

						child.setCurCoor(LIS_data.coor_);
						child.moveToChild();

						for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
						{
							bit_value = *child;

							if (bit_value[BS_SIZE_ - 2 - k] == 0)
							{
								output.push_back('0');
								LIP_.push_back(coorVal(child.coor(), *child));
							}
							else
							{
								output.push_back('1');

								if (*child >= 0)
									output.push_back('+');
								else
									output.push_back('-');

								LSP_.push_back(coorVal(child.coor(), *child));
							}
							
							if (i != (pow(2, this->dims_.size()) - 1))
								child.moveToNextSibling();
						}

						if (child.curLevel() != 0)
							LIS_.push_back(coorValTy(LIS_data.coor_, cType::typeB_));
					}
					else
					{
						output.push_back('0');
						TMP_LIS.push_back(LIS_data);
					}
				}
				else   // typeB_
				{
					child.setCurCoor(LIS_data.coor_);
					child.moveToChild();

					int one_flag = 0;
					for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
					{
						grand.setCurCoor(child.coor());
						grand.moveToChild();

						for (size_t j = 0; j < pow(2, this->dims_.size()); j++)
						{
							bit_value = *grand;

							if (bit_value[BS_SIZE_ - 2 - k] == 1)
							{
								one_flag = 1;
								break;
							}

							if (j != (pow(2, this->dims_.size()) - 1))
								grand.moveToNextSibling();
						}
						
						if (one_flag == 1)
							break;

						if (i != (pow(2, this->dims_.size()) - 1))
							child.moveToNextSibling();
					}

					if (one_flag == 1)
					{
						output.push_back('1');

						child.setCurCoor(LIS_data.coor_);
						child.moveToChild();

						for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
						{
							LIS_.push_back(coorValTy(child.coor(), cType::typeA_));

							if (i != (pow(2, this->dims_.size()) - 1))
								child.moveToNextSibling();
						}
					}
					else
					{
						output.push_back('0');
						TMP_LIS.push_back(LIS_data);
					}
				}
			}
			LIS_ = TMP_LIS;

			return output;
		}

		std::list<char> enRefinePass(size_t k, size_t LSP_size)
		{
			std::list<char> output;
			std::bitset<BS_SIZE_> bit_value;

			std::list<coorVal> TMP = LSP_;
			coorVal LSP_data;
			for (size_t i = 0; i < LSP_size; i++)
			{
				LSP_data = TMP.front();
				TMP.pop_front();

				bit_value = LSP_data.value_;

				if (bit_value[BS_SIZE_ - 2 - k] == 1)
					output.push_back('1');
				else
					output.push_back('0');
			}

			return output;
		}

		void deProgress(size_t step)
		{
			std::list<char> code;
			
			code = sig_output.front();
			sig_output.pop_front();
			deSigPass(step, code);

			code = ref_output.front();
			ref_output.pop_front();
			deRefinePass(step, code);
		}

		void deSigPass(size_t k, std::list<char> code)
		{
			std::bitset<BS_SIZE_> bit_value;
			char code_data;

			caWTIterator<Dty_, Ty_> it(this->data, this->dims_.size(), this->dims_);
			size_t LIP_size = LIP_.size();
			coorVal LIP_data;
			for (size_t i = 0; i < LIP_size; i++)
			{
				LIP_data = LIP_.front();
				LIP_.pop_front();

				code_data = code.front();
				code.pop_front();

				if (code_data == '1')
				{
					it.setCurCoor(LIP_data.coor_);
					bit_value = *it;

					bit_value[BS_SIZE_ - 2 - k] = 1;

					code_data = code.front();
					code.pop_front();

					if (code_data == '+')
						bit_value[BS_SIZE_ - 1] = 0;
					else
						bit_value[BS_SIZE_ - 1] = 1;

					*it = (Ty_)(bit_value.to_ulong);
					LSP_.push_back(LIP_data);
				}
				else
				{
					LIP_.push_back(LIP_data);
				}
			}

			caWTIterator<Dty_, Ty_> child(this->data, this->dims_.size(), this->dims_);
			std::list<coorValTy> TMP_LIS;
			coorValTy LIS_data;
			while (LIS_.size() != 0)
			{
				LIS_data = LIS_.front();
				LIS_.pop_front();

				code_data = code.front();
				code.pop_front();

				if (code_data == '1')
				{
					if (LIS_data.type_ == cType::typeA_)
					{
						child.setCurCoor(LIS_data.coor_);
						child.moveToChild();

						for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
						{
							code_data = code.front();
							code.pop_front();

							if (code_data == '1')
							{
								bit_value = *child;

								bit_value[BS_SIZE_ - 2 - k] = 1;

								code_data = code.front();
								code.pop_front();

								if (code_data == '+')
									bit_value[BS_SIZE_ - 1] = 0;
								else
									bit_value[BS_SIZE_ - 1] = 1;

								*child = (Ty_)(bit_value.to_ulong);
								LSP_.push_back(coorVal(child.coor(), *child));
							}
							else
							{
								LIP_.push_back(coorVal(child.coor(), *child));
							}

							if (i != (pow(2, this->dims_.size()) - 1))
								child.moveToNextSibling();
						}

						if (child.curLevel() != 0)
							LIS_.push_back(coorValTy(LIP_data.coor_, cType::typeB_));
					}
					else  // typeB_
					{
						child.setCurCoor(LIP_data.coor_);
						child.moveToChild();

						for (size_t i = 0; i < pow(2, this->dims_.size()); i++)
						{
							LIS_.push_back(coorValTy(child.coor(), cType::typeA_));

							if (i != (pow(2, this->dims_.size()) - 1))
								child.moveToNextSibling();
						}
					}
				}
				else
				{
					TMP_LIS.push_back(LIP_data);
				}
			}
			LIS_ = TMP_LIS;
		}

		void deRefinePass(size_t k, std::list<char> code)
		{
			std::list<coorVal> TMP = LSP_;
			std::bitset<BS_SIZE_> bit_value;
			char code_data;

			caWTIterator<Dty_, Ty_> it(this->data, this->dims_.size(), this->dims_);
			size_t code_size = code.size();
			coorVal LSP_data;
			for (size_t i = 0; i < code_size; i++)
			{
				LSP_data = TMP.front();
				TMP.pop_front();

				code_data = code.front();
				code.pop_front();

				if (code_data == '1')
				{
					it.setCurCoor(LSP_data.coor_);
					bit_value = *it;

					bit_value[BS_SIZE_ - 2 - k] = 1;
					*it = (Ty_)(bit_value.to_ulong);
				}
			}
		}


	private:
		void adjustCoor(caCoor<Dty_> coor, std::vector<Dty_> sP, std::vector<Dty_> eP) 
			throw(std::out_of_range);
		bool isExceed(caCoor<Dty_> coor, std::vector<Dty_> sP, std::vector<Dty_> eP)
			throw(std::out_of_range);
		void getBandSize(const std::vector<int>& dims, std::vector<int>& output, const size_t level)
		{
			double factor = pow(1 / 2, level);
			for (auto d : dims)
			{
				output.push_back(ceil(d * factor));
			}
		}

	private:
		enum class cType {typeA_, typeB_};

		typedef struct coorVal
		{
			caCoor<Dty_> coor_;		// Coordinate
			Ty_ value_;			// Value

			coorVal(caCoor<Dty_>& coor, Ty_ value) 
			{
				this->coor_ = coor;
				this->value_ = value;
			}
		}coorVal;

		typedef struct coorTy
		{
			caCoor<Dty_> coor_;		// Coordinate
			//double value_;			// Value
			cType type_;			// typeA_ or typeB_

			coorTy(caCoor<Dty_>& coor, cType type)
			{
				this->coor_ = coor;
				//this->value_ = value;
				this->type_ = type;
			}
		}coorValTy;

	private:
		size_t step_;
		size_t maxStep_;

		// wtData Info
		size_t wtLevel_;
		Ty_* data;
		std::vector<Dty_> dims_;

		std::list<coorValTy> LIS_;
		std::list<coorVal> LIP_;
		std::list<coorVal> LSP_;

		std::list<std::list<char>> sig_output;
		std::list<std::list<char>> ref_output;
	};
}

#endif // _caSPIHT_