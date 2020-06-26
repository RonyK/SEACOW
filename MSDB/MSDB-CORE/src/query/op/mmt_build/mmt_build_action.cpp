#include <op/mmt_build/mmt_build_action.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>

namespace msdb
{
mmt_build_action::mmt_build_action()
{
}

mmt_build_action::~mmt_build_action()
{
}

const char* mmt_build_action::name()
{
	return "mmt_build_action";
}

pArray mmt_build_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);
	// Get parameters
	// Param 0
	pArray arr = inputArrays[0];
	// Param 1
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);

	// Setup array
	arrayId arrId = arr->getArrayId();
	auto dims = arr->getDesc()->getDimDescs().getDims();
	auto chunkDims = arr->getDesc()->getDimDescs().getChunkDims();

	for (auto attr : arr->getDesc()->getAttrDescs())
	{
		pMMT mmtIndex = std::make_shared<mmt>(attr->type_, dims, chunkDims);
		auto it = arr->getChunkIterator(iterateMode::EXIST);
		mmtIndex->build(it);
		//arrayMgr::instance()->setAttributeIndex(arrId, attr->id_, mmtIndex);
	}

	return arr;
}
}	// msdb

