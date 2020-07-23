#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_

#include <array/arrayMgr.h>
#include <array/memChunk.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/mmt_delta_decode/mmt_delta_decode_array.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

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
						 std::shared_ptr< mmt_delta_encode_array> inArr,
						 pAttributeDesc attrDesc);

	template<class Ty_>
	void chunkDecode(pChunk oit,
					 pChunk iit,
					 std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex);
};

template<class Ty_>
void mmt_delta_decode_action::attributeDecode(std::shared_ptr<mmt_delta_decode_array> outArr, 
											  std::shared_ptr<mmt_delta_encode_array> inArr,
											  pAttributeDesc attrDesc)
{
	// Get MMT from mmt_delta_encode_array
	//auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getArrayId(), attrDesc->id_);
	auto arrIndex = inArr->getMMT(attrDesc->id_);

	if (arrIndex->getType() != attrIndexType::MMT)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
	}
	auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
	auto cit = inArr->getChunkIterator(iterateMode::EXIST);

	while (!cit.isEnd())
	{
		// Make new chunk
		auto cDesc = (*cit)->getDesc();
		pChunk deltaChunk = std::make_shared<memChunk>(std::make_shared<chunkDesc>(*cDesc));
		deltaChunk->alloc();

		this->chunkDecode(deltaChunk, *cit, mmtIndex);
		outArr->insertChunk(deltaChunk);
		++cit;
	}
}

template<class Ty_>
void mmt_delta_decode_action::chunkDecode(pChunk outChunk, pChunk inChunk,
										  std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex)
{
	auto nit = mmtIndex->getNodeIterator(0);	// get level 0 node iterator
	auto chunkDim = inChunk->getDesc()->dims_.data();
	auto blockDimInChunk = calcChunkNums(chunkDim,
										 mmtIndex->getLeaftBlockDim().data(),
										 inChunk->getDesc()->getDimSize());
	coorItr bit(blockDimInChunk);
	while (!bit.isEnd())
	{
		auto bItemBdy = mmtIndex->getBlockItemBoundary(bit.coor());
		auto iit = inChunk->getItemRangeIterator(bItemBdy);
		auto oit = outChunk->getItemRangeIterator(bItemBdy);
		auto node = mmtIndex->getNode(inChunk->getDesc()->chunkCoor_, bit.coor());

		// Block encode
		while (!iit.isEnd())
		{
			auto inValue = (*iit).get<Ty_>();
			auto outValue = inValue + node->min_;
			(*oit).set<Ty_>(outValue);
			++iit;
			++oit;
		}

		++bit;
	}
}
}	// msdb

#endif		// _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_