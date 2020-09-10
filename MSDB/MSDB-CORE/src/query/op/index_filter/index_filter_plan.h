#pragma once
#ifndef _MSDB_INDEX_FILTER_PLAN_H_
#define _MSDB_INDEX_FILTER_PLNA_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <parse/predicate.h>
#include <memory>
#include <list>

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
		size_t dSize = arrDesc->getDSize();

		auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrDesc->id_, attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto mmtLevel = mmtIndex->getMaxLevel();

		if(inPredicate->evaluateNode(mmtIndex->getNode(mmtLevel, 0)))		// Check root node
		{	
			// Target chunk exists
			// Start searching nodes
			std::vector<std::vector<bool>> nodes(mmtLevel + 1);
			size_t childs = (size_t)pow(2, dSize);
			
			//////////////////////////////
			// Level (mmtLevel)
			nodes[mmtLevel] = std::vector<bool>({ true });

			//////////////////////////////
			// Level (mmtLevel - 1)
			size_t curLevel = mmtLevel - 1;
			if(curLevel > 0)
			{
				auto nit = mmtIndex->getNodeIterator(curLevel);
				nodes[curLevel] = std::vector<bool>(nit.getCapacity(), false);

				while (!nit.isEnd())
				{
					if (inPredicate->evaluateNode((*nit)))
					{
						nodes[curLevel].push_back(true);
					} else
					{
						nodes[curLevel].push_back(false);
					}
					++nit;
				}
			}
			
			//////////////////////////////
			// Level (mmtLevel - 2) ~ 0
			for (curLevel -= 1; curLevel >= 0; --curLevel)
			{
				size_t prevLevel = curLevel + 1;
				auto pcit = mmtIndex->getNodeIterator(prevLevel);
				auto cit = mmtIndex->getNodeIterator(curLevel);
				nodes[curLevel] = std::vector<bool>(cit.getCapacity(), false);

				while (!pcit.isEnd())
				{
					// check parent
					if(nodes[prevLevel][pcit.seqPos()])
					{
						auto childBase = pcit.coor();
						for (dimensionId d = 0; d < dSize; ++d)
						{
							childBase[d] *= 2;
						}

						// Iterate childs
						for (size_t cID = 0; cID < childs; cID++)
						{
							// Set child coordinate and move to cit
							auto cur = childBase;
							for (dimensionId d = 0; d < dSize; ++d)
							{
								if (cID & ((dimensionId)0x1 << d))
								{
									cur[d] += 1;
								}
							}
							cit.moveTo(cur);

							if (inPredicate->evaluateNode((*cit)))
							{
								nodes[curLevel][cit.seqPos()] = true;
							} else
							{
								nodes[curLevel][cit.seqPos()] = false;
							}
						}
					}
					++pcit;
				}
			}

			// return bitmapTree
		}else
		{
			// No target chunk
			return std::make_shared<bitmapTree>(chunkSpace.area(), false);
		}
	}
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_PLAN_H_