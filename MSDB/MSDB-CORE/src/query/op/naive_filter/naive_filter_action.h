#pragma once
#ifndef _MSDB_OP_NAIVE_FILTER_ACTION_H_
#define _MSDB_OP_NAIVE_FILTER_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <parse/predicate.h>

namespace msdb
{
class naive_filter_action : public opAction
{
public:
	naive_filter_action();
	virtual ~naive_filter_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	template<class Ty_>
	void attributeFilter(pArray outArr, pArray inArr, pAttributeDesc attrDesc, pPredicate inPredicate)
	{
		auto inChunkItr = inArr->getChunkIterator();
		//auto outChunkItr = outArr->getChunkIterator();

		inPredicate->setEvaluateFunc(attrDesc->type_);

		while (!inChunkItr->isEnd()) 
		{
			if(inChunkItr->isExist())
			{
				auto inChunk = (**inChunkItr);
				auto outChunk = outArr->makeChunk(attrDesc->id_, inChunk->getId());
				outChunk->bufferRef(inChunk);

				auto isEmptyChunk = this->chunkFilter<Ty_>(outChunk, inChunk, inPredicate);
				if(isEmptyChunk)
				{
					outArr->freeChunk(inChunk->getId());
				}
			}
			++(*inChunkItr);
			//++(*outChunkItr);
		}
	}

	template <class Ty_>
	bool chunkFilter(pChunk outChunk, pChunk inChunk, pPredicate inPredicate)
	{
		bool isEmptyChunk = true;

		auto inBlockItr = inChunk->getBlockIterator();
		auto outBlockItr = outChunk->getBlockIterator();
		while (!inBlockItr->isEnd())
		{
			if(inBlockItr->isExist())
			{
				auto inBlock = (**inBlockItr);
				auto outBlock = outChunk->makeBlock(inBlock->getId());

				auto isEmptyBlock = this->blockFilter<Ty_>(outBlock, inBlock, inPredicate);
				if(isEmptyBlock)
				{
					outChunk->freeBlock(inBlock->getId());
				}else
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
				//std::cout << "setExist: "
				//	<< static_cast<int>((**outBlockItemItr).get<char>()) << std::endl;
				outBlockItemItr->setExist();
				isEmpty = false;
			}else
			{
				//std::cout << "setNull: "
				//	<< static_cast<int>((**outBlockItemItr).get<char>()) << std::endl;
				outBlockItemItr->setNull();
			}
			++(*inBlockItemItr);
			++(*outBlockItemItr);
		}

		return isEmpty;
	}
};
}

#endif // _MSDB_OP_NAIVE_FILTER_ACTION_H_
