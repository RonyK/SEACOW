#pragma once
#ifndef _MSDB_COMPASS_H_
#define _MSDB_COMPASS_H_

#include <stdafx.h>
#include <index/attributeIndex.h>
#include <util/element.h>
#include <array/chunkIterator.h>

namespace msdb
{
class compassIndex;
using pCompassIndex = std::shared_ptr<compassIndex>;

template <typename Ty_>
struct compassBin
{
public:
	std::vector<position_t> positional_;
	std::vector<Ty_> residual_;
};

class compassIndex : public attributeIndex
{
public:
	using size_type = size_t;

public:
	compassIndex(const eleType eType, const size_type numBins)
		: eType_(eType), numBins_(numBins)
	{}

public:
	static pCompassIndex createIndex(const eleType eType, const size_type numBins);

public:
	inline eleType getEleType()
	{
		return this->eType_;
	}
	inline size_type getNumBins()
	{
		return this->numBins_;
	}
	inline void setNumBins(size_type numBins)
	{
		this->numBins_ = numBins;
	}

public:
	virtual void build(pChunkIterator& it) = 0;

protected:
	eleType eType_;
	size_type numBins_;
};

template <typename Ty_>
class compassIndexImpl : public compassIndex
{
public:
	using size_type = size_t;

public:
	compassIndexImpl(const eleType eType, const size_type numBins)
		: compassIndex(eType, numBins)
	{}

private:
	inline Ty_ getBinValueRange()
	{
		// To prevent overflow at uint64_t
		// Pow with '_TySize - 1'
		// Make double at returning
		uint64_t maxValue = pow(2, _TySize_ - 1);
		return (maxValue / this->numBins_) * 2;
	}

public:
	void build(pChunkIterator& cit) override
	{
		this->arrayBins_.clear();
		this->arrayBins_.resize(cit->getCapacity());

		while (!cit->isEnd())
		{
			if(cit->isExist())
			{
				auto bit = (*cit)->getBlockIterator();

				while(!bit->isEnd())
				{
					if(bit->isExist())
					{
						this->buildBlockIndex(cit->seqPos(), **bit);
					}

					++(*bit);
				}
			}

			++(*cit);
		}
	}

	void buildBlockIndex(const chunkId cid, pBlock myBlock)
	{
		std::vector<compassBin<Ty_>>* blockBins = &(arrayBins_[cid][myBlock->getId()]);
		blockBins->resize(this->numBins_, compassBin<Ty_>());

		Ty_ binValueRange = this->getBinValueRange();
		assert(binValueRange != 0 && binValueRange > 0);

		auto iit = myBlock->getItemIterator();
		uint64_t negativeToPositive = pow(2, _TySize_ - 1);

		while (!iit->isEnd())
		{
			Ty_ value = (**iit).get<Ty_>();
			compassBin<Ty_>* curBin;
			if (!_TY_HAS_NEGATIVE_VALUE_)
			{
				assert((uint64_t)(value / binValueRange) < this->numBins_);
				curBin = &(blockBins->at((uint64_t)(value / binValueRange)));
			} else
			{
				assert((uint64_t)((value + negativeToPositive) / binValueRange) < this->numBins_);
				curBin = &(blockBins->at((uint64_t)((value + negativeToPositive) / binValueRange)));
			}

			curBin->positional_.push_back(iit->seqPos());
			curBin->residual_.push_back((value + negativeToPositive) % binValueRange);
			assert((value + negativeToPositive) % binValueRange >= 0);

			++(*iit);
		}
	}

private:
	std::vector<std::vector<std::vector<compassBin<Ty_>>>> arrayBins_;
};
}		// msdb
#endif	// _MSDB_COMPASS_H_