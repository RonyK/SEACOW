#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>
#include <cassert>

namespace msdb
{
mmt_build_plan::mmt_build_plan()
{
}

const char* mmt_build_plan::name()
{
	return "mmt_build_plan";
}

pAction mmt_build_plan::getAction()
{
	return std::make_shared<mmt_build_action>();
}

mmt_build_pset::mmt_build_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}

pArrayDesc mmt_build_pset::inferSchema()
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// Source array
	assert(this->params_[1]->type() == opParamType::CONST);		// Target level

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);
	
	return aInferDesc;
}
}	// msdb