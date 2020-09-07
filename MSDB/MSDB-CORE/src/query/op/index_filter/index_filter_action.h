#pragma once
#ifndef _MSDB_INDEX_FILTER_ACTION_H_
#define _MSDB_INDEX_FILTER_ACTION_H_

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
		auto inChunkItr = inArr->getChunkIterator();
		//auto outChunkItr = outArr->getChunkIterator();

		inPredicate->setEvaluateFunc(attrDesc->type_);

		while (!inChunkItr->isEnd())
		{
			if (inChunkItr->isExist())
			{
				auto inChunk = (**inChunkItr);
				auto outChunk = outArr->makeChunk(attrDesc->id_, inChunk->getId());
				outChunk->bufferRef(inChunk);
				this->chunkFilter<Ty_>(outChunk, inChunk, inPredicate);
			}
			++(*inChunkItr);
			//++(*outChunkItr);
		}
	}

	template <class Ty_>
	void chunkFilter(pChunk outChunk, pChunk inChunk, pPredicate inPredicate)
	{
		auto inBlockItr = inChunk->getBlockIterator();
		auto outBlockItr = outChunk->getBlockIterator();
		while (!inBlockItr->isEnd())
		{
			if (inBlockItr->isExist())
			{
				this->blockFilter<Ty_>((**outBlockItr), (**inBlockItr), inPredicate);
				++(*inBlockItr);
				++(*outBlockItr);
			}
		}
	}

	template <class Ty_>
	void blockFilter(pBlock outBlock, pBlock inBlock, pPredicate inPredicate)
	{
		auto inBlockItemItr = inBlock->getItemIterator();
		auto outBlockItemItr = outBlock->getItemIterator();

		while (!inBlockItemItr->isEnd())
		{
			if (inBlockItemItr->isExist() && inPredicate->evaluate(inBlockItemItr))
			{
				outBlockItemItr->isExist();
			}
			++(*inBlockItemItr);
			++(*outBlockItemItr);
		}
	}
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_ACTION_H_