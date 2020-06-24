#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_save/mmt_save_action.h>
#include <cassert>

namespace msdb
{
mmt_save_plan::mmt_save_plan()
{
}

const char* mmt_save_plan::name()
{
	return "mmt_save_plan";
}
pAction mmt_save_plan::getAction()
{
	return std::make_shared<mmt_save_action>();
}
mmt_save_pset::mmt_save_pset()
	: opParamSet()
{
}
mmt_save_pset::mmt_save_pset(parameters& pSet)
	: opParamSet(pSet)
{
}
pArrayDesc mmt_save_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}	// msdb
