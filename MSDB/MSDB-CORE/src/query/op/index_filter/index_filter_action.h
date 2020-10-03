#pragma once
#ifndef _MSDB_INDEX_FILTER_ACTION_H_
#define _MSDB_INDEX_FILTER_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <parse/predicate.h>

namespace msdb
{
class index_filter_action : public opAction
{
public:
	index_filter_action();
	virtual ~index_filter_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	template<class Ty_>
	void attributeFilter(pArray outArr, pArray inArr, pAttributeDesc attrDesc, pPredicate inPredicate)
	{
		inPredicate->setEvaluateFunc(attrDesc->type_);
		auto inChunkItr = inArr->getChunkIterator();

		std::stringstream ss;
		while (!inChunkItr->isEnd())
		{
			if (inChunkItr->isExist())
			{
				auto inChunk = (**inChunkItr);
				auto outChunk = outArr->makeChunk(attrDesc->id_, inChunk->getId());
				outChunk->bufferRef(inChunk);

				auto isEmptyChunk = this->chunkFilter<Ty_>(outChunk, inChunk, inPredicate);
				if (isEmptyChunk)
				{
					outArr->freeChunk(inChunk->getId());
					//BOOST_LOG_TRIVIAL(debug) << "[" << inChunk->getId() << "]: isEmpty";
				}

				//ss << "[" << inChunkItr->seqPos() << "]: true / ";
			}else
			{
				//ss << "[" << inChunkItr->seqPos() << "]: false / ";
			}
			
			++(*inChunkItr);
		}
		//BOOST_LOG_TRIVIAL(debug) << ss.str();
	}

	template <class Ty_>
	bool chunkFilter(pChunk outChunk, pChunk inChunk, pPredicate inPredicate)
	{
		bool isEmptyChunk = true;

		auto inBlockItr = inChunk->getBlockIterator();
		auto outBlockItr = outChunk->getBlockIterator();
		while (!inBlockItr->isEnd())
		{
			if (inBlockItr->isExist())
			{
				auto inBlock = (**inBlockItr);
				auto outBlock = outChunk->makeBlock(inBlock->getId());

				auto isEmptyBlock = this->blockFilter<Ty_>(outBlock, inBlock, inPredicate);
				if (isEmptyBlock)
				{
					outChunk->freeBlock(inBlock->getId());
				} else
				{
					isEmptyChunk = false;
				}

				++(*inBlockItr);
				++(*outBlockItr);
			}
		}

		return isEmptyChunk;
	}

	template <class Ty_>
	bool blockFilter(pBlock outBlock, pBlock inBlock, pPredicate inPredicate)
	{
		bool isEmpty = true;

		auto inBlockItemItr = inBlock->getItemIterator();
		auto outBlockItemItr = outBlock->getItemIterator();

		while (!inBlockItemItr->isEnd())
		{
			if (inBlockItemItr->isExist() && inPredicate->evaluate(inBlockItemItr))
			{
				outBlockItemItr->setExist();
				isEmpty = false;
			} else
			{
				outBlockItemItr->setNull();
			}
			++(*inBlockItemItr);
			++(*outBlockItemItr);
		}

		return isEmpty;
	}
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_ACTION_H_