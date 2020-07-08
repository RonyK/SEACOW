#include <op/load/load_plan.h>
#include <op/load/load_action.h>

namespace msdb
{
load_plan::load_plan()
{
}

load_plan::~load_plan()
{
}

const char* load_plan::name()
{
	return "load_plan";
}

pAction load_plan::getAction()
{
	return std::make_shared<load_action>();
}

load_pset::load_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc load_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb