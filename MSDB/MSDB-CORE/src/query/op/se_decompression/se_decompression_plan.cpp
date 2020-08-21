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
	: opArrayParamSet(pSet)
{

}

pArrayDesc se_decompression_pset::inferSchema()
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array
	assert(this->params_[1]->type() == opParamType::CONST);		// Target level

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	eleDefault level;
	std::static_pointer_cast<opParamConst::paramType>(this->params_[1]->getParam())->getData(&level);

	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
}	// msdb