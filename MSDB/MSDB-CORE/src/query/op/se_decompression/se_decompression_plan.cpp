#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

namespace msdb
{
se_decompression_plan::se_decompression_plan()
{
}

se_decompression_plan::~se_decompression_plan()
{
}

const char* se_decompression_plan::name()
{
	return "se_decompression_plan";
}

pAction se_decompression_plan::makeAction()
{
	return std::make_shared<se_decompression_action>();
}

se_decompression_array_pset::se_decompression_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array
	assert(this->params_[1]->type() == opParamType::CONST);		// Target level
}

pArrayDesc se_decompression_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	eleDefault level;
	std::static_pointer_cast<opParamConst::paramType>(this->params_[1]->getParam())->getData(&level);

	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmapTree se_decompression_array_pset::inferBottomUpBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();
	dimension blockSpace = desc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	return std::make_shared<bitmapTree>(seChunkSpace.area(), true);
}
}	// msdb