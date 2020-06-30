#include <op/mmt_load/mmt_load_action.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <system/storageMgr.h>

namespace msdb
{
mmt_load_action::mmt_load_action()
{
}

mmt_load_action::~mmt_load_action()
{
}

const char* mmt_load_action::name()
{
	return "mmt_load_action";
}

pArray mmt_load_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray arr = inputArrays[0];
	auto dims = arr->getDesc()->getDimDescs().getDims();
	auto chunkDims = arr->getDesc()->getDimDescs().getChunkDims();

	arrayId arrId = arr->getArrayId();

	for(auto attr : arr->getDesc()->getAttrDescs())
	{
		pMMT mmtIndex = MinMaxTree::createMMT(attr->type_, dims, chunkDims, chunkDims);		// maxLevel is not setted
		storageMgr::instance()->loadAttrIndex(arrId, attr->id_, mmtIndex);
		arrayMgr::instance()->setAttributeIndex(arrId, attr->id_, mmtIndex);
	}

	return pArray();
}
}	// msdb
