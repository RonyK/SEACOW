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

save_pset::save_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc save_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb