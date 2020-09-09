#pragma once
#ifndef _MSDB_INDEX_FILTER_PLAN_H_
#define _MSDB_INDEX_FILTER_PLNA_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <parse/predicate.h>
#include <memory>

namespace msdb
{
class index_filter_plan : public opPlan
{
public:
	index_filter_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class index_filter_array_pset : public opArrayParamSet
{
public:
	index_filter_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;

protected:
	template<typename Ty_>
	pBitmapTree inferBottomUpAttrBitmap(pArrayDesc arrDesc, pAttributeDesc attrDesc)
	{
		auto inPredicate = std::static_pointer_cast<predicate>(this->params_[1]->getParam());
		inPredicate->setEvaluateFunc(attrDesc->type_);

		dimension chunkSpace = arrDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = arrDesc->getDimDescs()->getBlockSpace();
		dimension mmtNodeSpace = chunkSpace * blockSpace;

		coorItr cit(chunkSpace);
		coorItr bit(blockSpace);

		auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrDesc->id_, attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto mmtLevel = mmtIndex->getMaxLevel();

		std::vector<bool> parentTargetNodes;
		std::vector<bool> curTargetNodes;
		for(int64_t level = mmtLevel; level > 0; --level)
		{
			curTargetNodes.clear();

			auto nit = mmtIndex->getNodeIterator(level);
			while(!nit.isEnd())
			{
				auto node = (*nit);
				//inPredicate->evaluate(node);
				++nit;
			}
		}
	}
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_PLAN_H_