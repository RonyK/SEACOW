#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

namespace msdb
{
mmt_delta_decode_plan::mmt_delta_decode_plan()
{
}

mmt_delta_decode_plan::~mmt_delta_decode_plan()
{
}

const char* mmt_delta_decode_plan::name()
{
	return "mmt_delta_decode";
}

pAction mmt_delta_decode_plan::getAction()
{
	return pAction();
}

mmt_delta_decode_pset::mmt_delta_decode_pset()
{
}

mmt_delta_decode_pset::mmt_delta_decode_pset(parameters& pSet)
	: opParamSet(pSet)
{

}

mmt_delta_decode_pset::~mmt_delta_decode_pset()
{
}

pArrayDesc mmt_delta_decode_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}
