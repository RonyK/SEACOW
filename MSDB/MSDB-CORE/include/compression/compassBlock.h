#pragma once
#ifndef _MSDB_COMPASSBLOCK_H_
#define _MSDB_COMPASSBLOCK_H_

#include <stdafx.h>
#include <array/memBlock.h>
#include <io/bitstream.h>

namespace msdb
{
class compassBlock;
using pCompassBlock = std::shared_ptr<compassBlock>;

class compassBlock : public memBlock
{
public:
	compassBlock(pBlockDesc desc);
	virtual ~compassBlock();

public:
	template <typename Ty_>
	struct bin
	{
	public:
		std::vector<position_t> positional_;
		std::vector<Ty_> residual_;
	};

public:
	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		std::vector<compassBlock::bin<Ty_>> bins_;
		this->buildBinFronValue<Ty_>(bins_);

		assert(bins_.size() == this->numBins_ && "number of bins comparison at SerializeTy");
		for(size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = bins_[bi];

			if (curBin.positional_.size() == 0)
			{
				// No values
				bs << setw(1) << 0;
			}else
			{
				// Value exist
				bs << setw(1) << 1;

				this->serializePositional(bs, curBin.positional_);
				this->serialzieResidual<Ty_>(bs, curBin.residual_);
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		std::vector<compassBlock::bin<Ty_>> bins_;
		bins_.resize(this->numBins_, bin<Ty_>());

		assert(bins_.size() == this->numBins_ && "number of bins comparison at deserializeTy");
		for(size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = bins_[bi];

			char isExist = false;
			bs >> setw(1);
			bs >> isExist;

			if(!isExist)
			{
				// No values
			}else
			{
				this->deserializePositional(bs, curBin.positional_);
				this->deserializeResidual<Ty_>(bs, curBin.residual_, curBin.positional_.size());
			}
		}

		this->buildArrayFromBin<Ty_>(bins_);
	}

private:
	template <typename Ty_>
	inline Ty_ getBinValueRange()
	{
		auto minValue = 0x0;			// 0000 0000
		auto maxValue = (Ty_)~(0);		// 1111 1111 - This will fill all bits with '1'

		return ((uint64_t)maxValue - minValue) / this->numBins_;
	}

	template <typename Ty_>
	void buildBinFronValue(std::vector<compassBlock::bin<Ty_>>& bins_)
	{
		bins_.resize(this->numBins_, bin<Ty_>());

		Ty_ binValueRange = this->getBinValueRange<Ty_>();
		auto iit = this->getItemIterator();

		while (!iit->isEnd())
		{
			Ty_ value = (**iit).get<Ty_>();
			auto curBin = bins_[(int64_t)(value / binValueRange)];
			curBin.positional_.push_back(iit->seqPos());
			curBin.residual_.push_back(value % binValueRange);

			++(*iit);
		}
	}

	template <typename Ty_>
	void buildArrayFromBin(std::vector<compassBlock::bin<Ty_>>& bins_)
	{
		Ty_ binValueRange = this->getBinValueRange<Ty_>();
		auto iit = this->getItemIterator();

		assert(bins_.size() == this->numBins_);
		for(size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = bins_[bi];
			Ty_ curBinBase = binValueRange * bi;
			assert(curBin.positional_.size() == curBin.residual_.size());

			for(size_t i = 0; i < curBin.positional_.size(); ++i)
			{
				auto pos = curBin.positional_[i];
				auto residual = curBin.residual_[i];

				auto arrValue = iit->getAtSeqPos(pos);
				arrValue.set<Ty_>(curBinBase + residual);
			}
		}
	}

	// Expects positional vector is ordered
	void serializePositional(bstream& bs, std::vector<position_t>& positional);
	void deserializePositional(bstream& bs, std::vector<position_t>& positional);
	
	template <typename Ty_>
	void serialzieResidual(bstream& bs, std::vector<Ty_>& residual)
	{
		position_t maxResidual = 0;
		for(auto r: residual)
		{
			if(maxResidual < r)
			{
				maxResidual = r;
			}
		}

		bs << setw(CHAR_BIT) << msb<position_t>(maxResidual);
		
		for(auto r: residual)
		{
			bs << r;
		}
	}

	template <typename Ty_>
	void deserializeResidual(bstream& bs, std::vector<Ty_>& residual, size_t numResiduals)
	{
		bit_cnt_type bMaxResidual = 0;
		Ty_ value;
		bs >> setw(CHAR_BIT) >> bMaxResidual;

		bs >> setw(bMaxResidual);

		while(numResiduals--)
		{
			bs >> value;
			residual.push_back(value);
		}
	}

public:
	void setNumBins_(size_t numBins);

private:
	size_t numBins_;
};
}

#endif	// _MSDB_COMPASSBLOCK_H_