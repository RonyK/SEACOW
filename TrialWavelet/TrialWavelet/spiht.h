#pragma once
#ifndef _caSPIHT_
#define _caSPIHT_

#include <iostream>
#include <vector>
#include <list>
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
				enProgress();
			}

			return 0;
		}
		int decode(std::vector<std::bitset<BS_SIZE_>>& spData, double& output)
		{
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
				this->LIP.push_back(coorVal(it.coor(), *it));
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
				this->LIS.push_back(coorValTy(coor, cType::typeA_));
			}
		}

		void enProgress()
		{
			enSigPass();
			enRefinePass();
		}

		void enSigPass()
		{

		}

		void enRefinePass()
		{

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
			double value_;			// Value

			coorVal(caCoor<Dty_>& coor, double value) 
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
	};
}

#endif // _caSPIHT_