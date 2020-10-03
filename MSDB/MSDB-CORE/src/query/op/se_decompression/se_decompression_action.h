#pragma once
#ifndef _MSDB_OP_SE_DECOMPRESSION_ACTION_H_
#define _MSDB_OP_SE_DECOMPRESSION_ACTION_H_

#include <stdafx.h>
#include <array/arrayMgr.h>
#include <array/memBlock.h>
#include <system/storageMgr.h>
#include <compression/wtChunk.h>
#include <compression/seChunk.h>
#include <compression/waveletUtil.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
class se_decompression_action : public opAction
{
public:
	se_decompression_action();
	~se_decompression_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;

private:
	pSeChunk makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
						  chunkId cid, coor chunkCoor);

	template <typename Ty_>
	void decompressAttribute(std::shared_ptr<wavelet_encode_array>outArr, pAttributeDesc attrDesc, pQuery qry)
	{
		auto arrIndex = arrayMgr::instance()->getAttributeIndex(outArr->getId(), attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto cit = outArr->getChunkIterator(iterateMode::ALL);
		auto maxLevel = outArr->getMaxLevel();

		bool hasNegative = false;
		if ((Ty_)-1 < 0)
		{
			hasNegative = true;
		}

		while (!cit->isEnd())
		{
			if(cit->isExist())
			{
				// make chunk
				chunkId cid = cit->seqPos();
				coor chunkCoor = cit->coor();
				auto inChunk = this->makeInChunk(outArr, attrDesc, cid, chunkCoor);

				auto blockBitmap = this->getPlanBlockBitmap(cid);
				if (blockBitmap)
				{
					inChunk->copyBlockBitmap(blockBitmap);
				} else
				{
					// If there were no bitmap, set all blocks as true.
					inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
				}
				inChunk->makeAllBlocks();
				// TODO::Create se_compression_array, seChunk
				// Make seChunk in se_compression_array

				this->decompressChunk(inChunk, mmtIndex, maxLevel, hasNegative);
				//========================================//
				qry->getTimer()->nextWork(0, workType::IO);
				pSerializable serialChunk
					= std::static_pointer_cast<serializable>(inChunk);
				storageMgr::instance()->loadChunk(outArr->getId(), attrDesc->id_, inChunk->getId(),
												  serialChunk);

				//========================================//
				qry->getTimer()->nextWork(0, workType::COMPUTING);
				auto outChunk = outArr->makeChunk(*inChunk->getDesc());
				auto wtOutChunk = std::static_pointer_cast<wtChunk>(outChunk);
				wtOutChunk->setLevel(inChunk->getLevel());
				wtOutChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
				wtOutChunk->makeAllBlocks();
				outChunk->bufferRef(inChunk);
			}

			++(*cit);
		}
	}

	template <typename Ty_>
	void decompressChunk(pSeChunk inChunk, 
						 std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
						 size_t maxLevel,
						 bool hasNegative)
	{
		auto dSize = inChunk->getChunkCoor().size();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;

		// For Level 0
		this->findRequiredBitsForRootLevel(inChunk,
										   mmtIndex,
										   numBandsInLevel, 
										   hasNegative);

		// For child level
		this->findRequiredBitsForChildLevel(inChunk,
											mmtIndex,
											maxLevel,
											numBandsInLevel,
											hasNegative);
	}

	template <typename Ty_>
	void findRequiredBitsForRootLevel(pSeChunk inChunk, 
									  std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
									  const size_t numBandsInLevel,
									  const bool hasNegative)
	{
		auto chunkCoor = inChunk->getChunkCoor();
		auto blockLevel = mmtIndex->getBlockLevel();
		auto mNode = mmtIndex->getNode(chunkCoor, blockLevel);
		bit_cnt_type fromMMT = getRBitFromMMT(mNode, hasNegative);

		for (size_t band = 0; band <= numBandsInLevel; ++band)
		{
			inChunk->rBitFromMMT.push_back(fromMMT);
		}
	}

	template <typename Ty_>
	void findRequiredBitsForChildLevel(pSeChunk inChunk,
									   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
									   const size_t maxLevel,
									   const size_t numBandsInLevel,
									   const bool hasNegative)
	{
		auto dSize = inChunk->getChunkCoor().size();
		for (size_t level = 1; level <= maxLevel; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			coorItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + inChunk->getChunkCoor() * innerSpace);
				auto mNode = mmtIndex->getNode(innerCoor, mmtIndex->getBlockLevel() - level);
				bit_cnt_type rbFromMMT = getRBitFromMMT(mNode, hasNegative);
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					inChunk->rBitFromMMT.push_back(rbFromMMT);

//#ifndef NDEBUG
//					BOOST_LOG_TRIVIAL(trace) << "level: " << level << ", band: " << band;
//					BOOST_LOG_TRIVIAL(trace) << mNode->toString<Ty_>();
//#endif
				}

				++innerItr;
			}
		}
	}
};
}	// msdb
#endif		// _MSDB_OP_SE_DECOMPRESSION_ACTION_H_