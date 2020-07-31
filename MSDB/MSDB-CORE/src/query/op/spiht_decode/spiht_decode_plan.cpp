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
	return "spiht_decode";
}

pAction spiht_decode_plan::getAction()
{
	return std::make_shared<spiht_decode_action>();
}

// pset
spiht_decode_pset::spiht_decode_pset()
	: opParamSet()
{
}

spiht_decode_pset::spiht_decode_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc spiht_decode_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);

	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}