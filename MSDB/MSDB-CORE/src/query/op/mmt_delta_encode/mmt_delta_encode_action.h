#pragma once
#ifndef _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_
#define _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_

#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <query/opAction.h>
#include <memory>

namespace msdb
{
class mmt_delta_encode_action : public opAction
{
public:
	mmt_delta_encode_action();
	~mmt_delta_encode_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

	template<class Ty_>
	void attributeEncode(pArray inArr, pArray outArr, pAttributeDesc attrDesc);
};
template<class Ty_>
inline void mmt_delta_encode_action::attributeEncode(pArray outArr, pArray inArr, pAttributeDesc attrDesc)
{
	auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getArrayId(), attrDesc->id_);
	if (arrIndex->getType() != attrIndexType::MMT)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
	}
	auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
	auto iit = inArr->getChunkIterator(iterateMode::EXIST);
	auto oit = outArr->getChunkIterator();

	mmtIndex->deltaEncode(oit, iit);
}
}

#endif		// _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_