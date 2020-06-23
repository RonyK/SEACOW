#include <op/mmt_encode/mmt_encode_plan.h>
#include <op/mmt_encode/mmt_encode_action.h>
#include <cassert>

namespace msdb
{
mmt_encode_plan::mmt_encode_plan()
{
}

const char* mmt_encode_plan::name()
{
	return "mmt_encode_plan";
}
pAction mmt_encode_plan::getAction()
{
	return std::make_shared<mmt_encode_action>();
}
mmt_encode_pset::mmt_encode_pset()
	: opParamSet()
{
}
mmt_encode_pset::mmt_encode_pset(parameters& pSet)
	: opParamSet(pSet)
{
}
pArrayDesc mmt_encode_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}	// msdb
