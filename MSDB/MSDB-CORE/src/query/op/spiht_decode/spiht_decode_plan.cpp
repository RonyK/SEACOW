#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/spiht_decode/spiht_decode_action.h>
#include <cassert>

namespace msdb
{
// plan
spiht_decode_plan::spiht_decode_plan()
{
}

const char* spiht_decode_plan::name()
{
	return "spiht_decode_plan";
}

pAction spiht_decode_plan::makeAction()
{
	return std::make_shared<spiht_decode_action>();
}

// pset
spiht_decode_array_pset::spiht_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
pBitmapTree spiht_decode_array_pset::inferBottomUpBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();
	dimension blockSpace = desc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	return std::make_shared<bitmapTree>(seChunkSpace.area(), true);
}
spiht_decode_plan_pset::spiht_decode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
}
}