#pragma once
#ifndef _MSDB_OP_SE_DECOMPRESSION_ACTION_H_
#define _MSDB_OP_SE_DECOMPRESSION_ACTION_H_

#include <array/arrayMgr.h>
#include <array/memBlock.h>
#include <system/storageMgr.h>
#include <compression/wtChunk.h>
#include <compression/seChunk.h>
#include <compression/waveletUtil.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <memory>
#include <list>

namespace msdb
{
class se_decompression_action : public opAction
{
public:
	se_decompression_action();
	~se_decompression_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	pSeChunk makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
						  chunkId cid, coor chunkCoor);

	template <typename Ty_>
	void decompressAttribute(std::shared_ptr<wavelet_encode_array>outArr, pAttributeDesc attrDesc)
	{
		auto arrIndex = arrayMgr::instance()->getAttributeIndex(outArr->getId(), attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		bool hasNegative = false;
		if ((Ty_)-1 < 0)
		{
			hasNegative = true;
		}

		while (!cit->isEnd())
		{
			// make chunk
			chunkId cid = cit->seqPos();
			coor chunkCoor = cit->coor();
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid, chunkCoor);

			auto mNode = mmtIndex->getNode(chunkCoor, 0);
			inChunk->rBitFromMMT = getRBitFromMMT(mNode) + static_cast<char>(hasNegative);

			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(inChunk);
			storageMgr::instance()->loadChunk(outArr->getId(), attrDesc->id_, inChunk->getId(),
											  serialChunk);

			//std::cout << "inChunk:: " << std::endl;
			//inChunk->print();

			auto outChunk = outArr->makeChunk(*inChunk->getDesc());
			auto wtOutChunk = std::static_pointer_cast<wtChunk>(outChunk);
			wtOutChunk->setLevel(inChunk->getLevel());
			wtOutChunk->makeAllBlocks();
			outChunk->bufferCopy(inChunk);

			++(*cit);
		}
	}
};
}	// msdb
#endif		// _MSDB_OP_SE_DECOMPRESSION_ACTION_H_