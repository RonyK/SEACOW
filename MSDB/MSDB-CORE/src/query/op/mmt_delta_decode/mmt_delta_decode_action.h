#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_

#include <stdafx.h>
#include <array/arrayMgr.h>
#include <array/blockChunk.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/mmt_delta_decode/mmt_delta_decode_array.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>
#include <util/threadUtil.h>

namespace msdb
{
class mmt_delta_decode_action : public opAction
{
public:
	mmt_delta_decode_action();
	~mmt_delta_decode_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

	template<class Ty_>
	void attributeDecode(std::shared_ptr<mmt_delta_decode_array> outArr, 
						 pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{
		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		// Get MMT from mmt_delta_encode_array
		//auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getArrayId(), attrDesc->id_);
		auto attrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
		if (attrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto arrMMTIndex = std::static_pointer_cast<mmt>(attrIndex);
		//inArr->setMMT(attrDesc->id_, arrMMTIndex);
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrMMTIndex);
		auto cit = inArr->getChunkIterator(iterateMode::EXIST);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				// Make new chunk
				auto cDesc = (**cit)->getDesc();
				//pChunk deltaChunk = std::make_shared<memBlockChunk>(std::make_shared<chunkDesc>(*cDesc));
				pChunk deltaChunk = outArr->makeChunk(*cDesc);
				//deltaChunk->makeAllBlocks();
				deltaChunk->replaceBlockBitmap((**cit)->getBlockBitmap());
				deltaChunk->bufferAlloc();

				io_service_->post(boost::bind(&mmt_delta_decode_action::chunkDecode<Ty_>, this,
								  deltaChunk, **cit, mmtIndex, qry, currentThreadId));

				//this->chunkDecode(deltaChunk, **cit, mmtIndex);
				//outArr->insertChunk(deltaChunk);
			}

			++(*cit);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//
	}

	template<class Ty_>
	void chunkDecode(pChunk outChunk,
					 pChunk inChunk,
					 std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
					 pQuery qry,
					 const size_t parentThreadId)
	{
		auto threadId = getThreadId();
		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//----------------------------------------//

		auto ibItr = inChunk->getBlockIterator();
		auto obItr = outChunk->getBlockIterator();
		while (!ibItr->isEnd())
		{
			if (ibItr->isExist())
			{
				outChunk->makeBlock(ibItr->seqPos());

				auto iit = (**ibItr)->getItemIterator();
				auto oit = (**obItr)->getItemIterator();
				auto node = mmtIndex->getNode(inChunk->getDesc()->chunkCoor_, ibItr->coor(), mmtIndex->getBlockLevel());

				// Block encode
				while (!iit->isEnd())
				{
					auto inValue = (**iit).get<Ty_>();
					auto outValue = inValue + node->getMin<Ty_>();
					(**oit).set<Ty_>(outValue);
					++(*iit);
					++(*oit);
				}
			}

			++(*ibItr);
			++(*obItr);
		}

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}
};
}	// msdb

#endif		// _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_