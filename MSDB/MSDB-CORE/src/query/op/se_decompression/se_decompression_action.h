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

		while (!cit->isEnd())
		{
			// make chunk
			chunkId cid = cit->seqPos();
			coor chunkCoor = cit->coor();
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid, chunkCoor);
			inChunk->bufferAlloc();
			inChunk->makeAllBlocks();

			auto curMMTNode = mmtIndex->getNode(chunkCoor, 0);
			inChunk->rBitFromMMT = curMMTNode->bits_;

			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(inChunk);
			storageMgr::instance()->loadChunk(outArr->getId(), attrDesc->id_, inChunk->getId(),
											  serialChunk);

			std::cout << "inChunk:: " << std::endl;
			inChunk->print();

			auto outChunk = std::make_shared<wtChunk>(inChunk->getDesc());
			outChunk->setLevel(inChunk->getLevel());
			outChunk->makeAllBlocks();
			outChunk->bufferCopy(inChunk);
			outArr->insertChunk(outChunk);

			++(*cit);
		}
	}

	//template<typename Ty_>
	//void decompressAttribute(std::shared_ptr<wavelet_encode_array>inArr, pAttributeDesc attrDesc)
	//{
	//	auto arrId = inArr->getId();
	//	auto cit = inArr->getChunkIterator(iterateMode::EXIST);

	//	auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
	//	if (arrIndex->getType() != attrIndexType::MMT)
	//	{
	//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
	//	}
	//	auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
	//	dimension chunkDim(inArr->getDesc()->getDimDescs().getChunkDims());

	//	while (!cit->isEnd())
	//	{
	//		pChunk outChunk = this->decompressChunk<Ty_>(
	//			std::static_pointer_cast<wtChunk>(**cit), mmtIndex, chunkDim);

	//		auto attr = outChunk->getDesc()->attrDesc_;
	//		storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
	//										  std::static_pointer_cast<serializable>(outChunk));
	//		++(*cit);
	//	}
	//}

	//template<typename Ty_>
	//pSeChunk decompressChunk(pWtChunk inChunk,
	//					   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
	//					   dimension& sourceChunkDim)
	//{
	//	size_t dSize = inChunk->getDSize();
	//	pWtChunk outChunk = this->makeOutChunk(inChunk);
	//	pBlock outBlock = outChunk->getBlock(0);

	//	size_t numBandsInLevel = std::pow(2, dSize) - 1;
	//	dimension inBlockDims = inChunk->getDesc()->getBlockDims();
	//	dimension bandDims = inBlockDims / std::pow(2, inChunk->getLevel() + 1);

	//	// Do band 0 in max level
	//	{
	//		bit_cnt_type requiredBits = this->decompressBand<Ty_>(outBlock, getBandRange(0, bandDims));
	//		outChunk->rBitFromDelta.push_back(requiredBits);
	//	}

	//	for (size_t level = 0; level <= inChunk->getLevel(); ++level)
	//	{
	//		for (size_t band = 1; band <= numBandsInLevel; ++band)
	//		{
	//			bit_cnt_type requiredBits = this->decompressBand<Ty_>(outBlock, getBandRange(band, bandDims));
	//			outChunk->rBitFromDelta.push_back(requiredBits);
	//		}

	//		bandDims *= 2;
	//	}

	//	return outChunk;
	//}

	//template <class Ty_>
	//bit_cnt_type decompressBand(pBlock curBlock, const coorRange bandRange)
	//{
	//	auto bItemItr = curBlock->getItemRangeIterator(bandRange);
	//	bit_cnt_type maxValueBits = 0;

	//	while (!bItemItr->isEnd())
	//	{
	//		bit_cnt_type valueBits = msb<bit_cnt_type>(abs_((**bItemItr).get<Ty_>()));
	//		if (maxValueBits < valueBits)
	//		{
	//			maxValueBits = valueBits;
	//		}
	//		++(*bItemItr);
	//	}

	//	return maxValueBits;
	//}
};
}	// msdb
#endif		// _MSDB_OP_SE_DECOMPRESSION_ACTION_H_