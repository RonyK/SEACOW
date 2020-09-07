#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

namespace msdb
{
index_filter_plan::index_filter_plan()
{
}

const char* index_filter_plan::name()
{
	return "index_filter";
}

pAction index_filter_plan::makeAction()
{
	return std::make_shared<index_filter_action>();
}

index_filter_array_pset::index_filter_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::PREDICATE);
}

pArrayDesc index_filter_array_pset::inferSchema()
{
	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}