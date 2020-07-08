#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <cassert>

namespace msdb
{
// plan
spiht_encode_plan::spiht_encode_plan()
{
}

const char* spiht_encode_plan::name()
{
	return "spiht_encode";
}

pAction spiht_encode_plan::getAction()
{
	return std::make_shared<spiht_encode_action>(); 
}

// pset
spiht_encode_pset::spiht_encode_pset()
	: opParamSet()
{
}

spiht_encode_pset::spiht_encode_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc spiht_encode_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// Source array
	
	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}