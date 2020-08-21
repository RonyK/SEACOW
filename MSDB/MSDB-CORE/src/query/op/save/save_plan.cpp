#include <op/save/save_plan.h>
#include <op/save/save_action.h>

namespace msdb
{
save_plan::save_plan()
{
}

save_plan::~save_plan()
{
}

const char* save_plan::name()
{
	return "save_plan";
}

pAction save_plan::getAction()
{
	return std::make_shared<save_action>();
}

save_array_pset::save_array_pset(parameters& pSet)
	: opParamSet(pSet)
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array
}

pArrayDesc save_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(
		this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmap save_array_pset::inferBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();
	
	return std::make_shared<bitmap>(chunkSpace.area(), true);
}
save_plan_pset::save_plan_pset(parameters& pSet)
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::PLAN);		// source plan
}
pArrayDesc save_plan_pset::inferSchema()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	return std::make_shared<arrayDesc>(*sourcePlan->inferSchema());
}
pBitmap save_plan_pset::inferBitmap()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	return std::make_shared<bitmap>(*sourcePlan->inferBitmap());
}
}	// msdb