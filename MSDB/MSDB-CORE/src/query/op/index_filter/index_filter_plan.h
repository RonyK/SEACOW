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
//////////////////////////////
// index_filter_plan
class index_filter_plan : public opPlan
{
public:
	index_filter_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// index_filter_array_pset
//
// Base class for pSet class of index_filter operator
class index_filter_pset
{
protected:
	template<typename Ty_>
	pBitmapTree inferBottomUpAttrBitmap(pArrayDesc arrDesc, pAttributeDesc attrDesc, pPredicate inPredicate)
	{
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
		auto pMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto mmtLevel = pMmtIndex->getMaxLevel();

		if (inPredicate->evaluateNode(pMmtIndex->getNode(0, mmtLevel)))		// Check root node
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
			int64_t curLevel = mmtLevel - 1;
			if (curLevel >= 0)
			{
				this->inferBoUpBitmapFirstLevel(inPredicate, pMmtIndex, nodes, curLevel, dSize);
			}

			//////////////////////////////
			// Level (mmtLevel - 2) ~ 0
			curLevel -= 1;
			while (curLevel >= 0)
			{
				this->inferBoUpBitmapChildLevel(inPredicate, pMmtIndex, nodes, curLevel, dSize);
				curLevel -= 1;
			}

			return this->inferBoUpBitmapChildLevel(nodes, pMmtIndex,
												   chunkSpace, blockSpace);
		} else
		{
			// No target chunk
			return std::make_shared<bitmapTree>(chunkSpace.area(), false);
		}
	}

	template <typename Ty_>
	void inferBoUpBitmapFirstLevel(std::shared_ptr<predicate> inPredicate,
								   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>>pMmtIndex,
								   std::vector<std::vector<bool>>& nodes,
								   size_t curLevel, size_t dSize)
	{
		auto nit = pMmtIndex->getNodeIterator(curLevel);
		nodes[curLevel] = std::vector<bool>(nit.getCapacity(), false);

		while (!nit.isEnd())
		{
			if (inPredicate->evaluateNode((*nit)))
			{
				nodes[curLevel][nit.seqPos()] = true;
			} else
			{
				nodes[curLevel][nit.seqPos()] = false;
			}
			++nit;
		}
	}

	template <typename Ty_>
	void inferBoUpBitmapChildLevel(std::shared_ptr<predicate> inPredicate,
								   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>>pMmtIndex,
								   std::vector<std::vector<bool>>& nodes,
								   size_t curLevel, size_t dSize)
	{
		size_t prevLevel = curLevel + 1;
		size_t childs = (size_t)pow(2, dSize);
		auto pcit = pMmtIndex->getNodeIterator(prevLevel);
		auto cit = pMmtIndex->getNodeIterator(curLevel);
		nodes[curLevel] = std::vector<bool>(cit.getCapacity(), false);

		while (!pcit.isEnd())
		{
			// check parent
			if (nodes[prevLevel][pcit.seqPos()])
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

	template <typename Ty_>
	pBitmapTree inferBoUpBitmapChildLevel(std::vector<std::vector<bool>>& nodes,
										  std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>>pMmtIndex,
										  dimension& chunkSpace, dimension& blockSpace)
	{
		size_t chunkNums = chunkSpace.area();
		size_t blockNums = blockSpace.area();
		size_t chunkLevel = pMmtIndex->getChunkLevel();
		size_t blockLevel = pMmtIndex->getBlockLevel();

		auto chunkBitmap = std::make_shared<bitmapTree>(chunkNums, false);

		for (chunkId cid = 0; cid < chunkNums; ++cid)
		{
			if (nodes[chunkLevel][cid])
			{
				chunkBitmap->setExist(cid);
				size_t offset = cid * blockNums;
				auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, false);
				for (blockId bid = 0; bid < blockNums; ++bid)
				{
					if (nodes[blockLevel][offset + bid])
					{
						blockBitmap->setExist(bid);
					}
				}
			}
		}

		return chunkBitmap;
	}
};

//////////////////////////////
// index_filter_array_pset
class index_filter_array_pset : public opArrayParamSet, public index_filter_pset
{
public:
	index_filter_array_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};

//////////////////////////////
// index_filter_plan_pset
class index_filter_plan_pset : public opPlanParamSet, public index_filter_pset
{
public:
	index_filter_plan_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_PLAN_H_