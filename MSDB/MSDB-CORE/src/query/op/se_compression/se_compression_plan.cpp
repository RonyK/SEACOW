#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_compression_action.h>

namespace msdb
{
se_compression_plan::se_compression_plan()
{
}

se_compression_plan::~se_compression_plan()
{
}

const char* se_compression_plan::name()
{
	return "se_compression_plan";
}

pAction se_compression_plan::getAction()
{
	return std::make_shared<se_compression_action>();
}

se_compression_pset::se_compression_pset(parameters& pSet)
	: opParamSet(pSet)
{
}

pArrayDesc se_compression_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb