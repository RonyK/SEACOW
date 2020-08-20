#include <array/arrayMgr.h>
#include <system/storageMgr.h>
#include <op/mmt_save/mmt_save_action.h>
#include <index/mmt.h>
#include <memory>

namespace msdb
{
mmt_save_action::mmt_save_action()
{
}

mmt_save_action::~mmt_save_action()
{
}

const char* mmt_save_action::name()
{
	return "mmt_save_action";
}

pArray mmt_save_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	pArray arr = inputArrays[0];
	arrayId arrId = arr->getId();

	for (auto attr : *arr->getDesc()->attrDescs_)
	{
		auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrId, attr->id_);
		if(arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		pMMT mmtIndex = std::static_pointer_cast<mmt>(arrIndex);
		// dynamic_pointer_cast / C2683
		storageMgr::instance()->saveAttrIndex(arrId, attr->id_, mmtIndex);
	}

	return arr;
}
}
