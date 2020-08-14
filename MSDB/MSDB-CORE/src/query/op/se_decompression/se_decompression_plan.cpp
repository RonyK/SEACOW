#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

namespace msdb
{
se_decompression_plan::se_decompression_plan()
{
}

se_decompression_plan::~se_decompression_plan()
{
}

const char* se_decompression_plan::name()
{
	return "se_decompression_plan";
}

pAction se_decompression_plan::getAction()
{
	return std::make_shared<se_decompression_action>();
}

se_decompression_pset::se_decompression_pset(parameters& pSet)
	: opParamSet(pSet)
{

}

pArrayDesc se_decompression_pset::inferSchema()
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb