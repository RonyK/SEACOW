#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>

namespace msdb
{
mmt_delta_encode_plan::mmt_delta_encode_plan()
{
}

mmt_delta_encode_plan::~mmt_delta_encode_plan()
{
}

const char* mmt_delta_encode_plan::name()
{
	return "mmt_delta_encode_plan";
}

pAction mmt_delta_encode_plan::getAction()
{
	return std::make_shared<mmt_delta_encode_action>();
}

mmt_delta_encode_pset::mmt_delta_encode_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc mmt_delta_encode_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb