#pragma once
#ifndef _MSDB_OP_SE_COMPRESSION_ACTION_H_
#define _MSDB_OP_SE_COMPRESSION_ACTION_H_

#include <array/arrayMgr.h>
#include <array/memBlock.h>
#include <system/storageMgr.h>
#include <compression/wtChunk.h>
#include <compression/seChunk.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

#include <memory>
#include <list>

namespace msdb
{
class se_compression_action : public opAction
{
public:
	se_compression_action();
	~se_compression_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

public:
	template<typename Ty_>
	void compressAttribute(std::shared_ptr<wavelet_encode_array>inArr, pAttributeDesc attrDesc)
	{
		auto arrId = inArr->getId();
		auto cit = inArr->getChunkIterator(iterateMode::EXIST);

		auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		dimension chunkDim(inArr->getDesc()->getDimDescs().getChunkDims());

		while (!cit->isEnd())
		{
			std::vector<pWtChunk> wtChunks;
			std::vector<pSeChunk> outChunks;
			// Combine all chunks derived from a single source chunk.

			auto numChunks = 1 + (inArr->getMaxLevel() + 1) * (pow(2, inArr->getDesc()->getDSize()) - 1);
			// approximate 1 + detail part

			for (size_t i = 0; i < 1 + (inArr->getMaxLevel() + 1) * (pow(2, inArr->getDesc()->getDSize()) - 1); ++i)
			{
				if(cit->isEnd())
				{
					_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "iterating chunk fail");
				}
				wtChunks.push_back(std::static_pointer_cast<wtChunk>(**cit));
				++(*cit);
			}

			this->compressChunk<Ty_>(outChunks, wtChunks, mmtIndex, chunkDim);

			for (auto outChunk : outChunks)
			{
				auto attr = outChunk->getDesc()->attrDesc_;
				storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
												  std::static_pointer_cast<serializable>(outChunk));
			}
		}
	}

	template<typename Ty_>
	void compressChunk(std::vector<pSeChunk>& out, std::vector<pWtChunk>& in,
					   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
					   dimension& sourceChunkDim)
	{
		assert(in.size() >= 2);
		pWtChunk iChunk = in[0];	// approximate chunk, get tile dim
		dimension blockDims = iChunk->getDesc()->getDim();	// TODO::FIX, now simple approch

		for(auto c : in)
		{
			auto iDesc = c->getDesc();
			auto oDesc = std::make_shared<blockChunkDesc>(iDesc->id_,
														  std::make_shared<attributeDesc>(*(iDesc->attrDesc_)),
														  iDesc->dims_, blockDims,
														  iDesc->sp_, iDesc->ep_,
														  iDesc->mSize_);
			pSeChunk oChunk = std::make_shared<seChunk>(oDesc);
			oChunk->setLevel(iChunk->getLevel());
			oChunk->setBandId(iChunk->getBandId());
			oChunk->setSourceChunkId(iChunk->getSourceChunkId());
			oChunk->makeAllBlocks();
			oChunk->alloc();
 
			//dimension blockSpace = c->getTileSpace(sourceChunkDim);
			//dimension blockSpace = c->getDesc()->getBlockSpace();
			//coorItr blockItr(blockSpace);
			// TODO::Use block iterator
			auto ibItr = c->getBlockIterator();
			//auto obItr = oChunk->getBlockIterator();

			while (!(ibItr->isEnd()))
			{
				coor blockCoor = ibItr->coor();
				coor blockSp = blockCoor * blockDims;
				coor blockEp = blockSp + blockDims;
				coorRange bRange = coorRange(blockSp, blockEp);

				//pBlockDesc bDesc = std::make_shared<blockDesc>(
				//	blockItr.seqPos(),					// id
				//	iDesc->attrDesc_->type_,			// eType
				//	blockDims,							// dims
				//	blockSp,							// sp
				//	blockEp,							// ep
				//	blockDims.area() * c->getDesc()->attrDesc_->typeSize_	// mSize
				//	);
				//pBlock oBlock = std::make_shared<memBlock>(bDesc);
				//oChunk->setBlock(oBlock);

				//////////////////////////////
				// TODO::Use materialize copy
				// oBlock->materializeCopy(void*, blockDims.area() * c->getDesc()->attrDesc_->typeSize_);
				//oBlock->alloc();	// mSize is setted in the desc.
				//////////////////////////////

				// Find required bit for delta array
				auto iBlockItemItr = (*ibItr)->getItemIterator();
				auto oBlockItemItr = oChunk->getBlock((*ibItr)->getId())->getItemIterator();

				bit_cnt_type maxValueBits = 0;
				while(!iBlockItemItr->isEnd())
				{
					bit_cnt_type valueBits = msb<bit_cnt_type>(abs_((**iBlockItemItr).get<Ty_>()));
					if(maxValueBits < valueBits)
					{
						maxValueBits = valueBits;
					}

					// As input/output block have same type, they can access the same coordinate using the next() operator.
					(**oBlockItemItr) = (**iBlockItemItr);
					++(*iBlockItemItr);
					++(*oBlockItemItr);	 
					
				}
				oChunk->rBitFromDelta.push_back(maxValueBits);

				auto mmtNode = mmtIndex->getNode(bRange);
				oChunk->rBitFromMMT.push_back(mmtNode->bits_);

				++(*ibItr);
			}

			out.push_back(oChunk);
		}
	}
};
}	// msdb
#endif		// _MSDB_OP_SE_COMPRESSION_ACTION_H_