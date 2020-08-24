#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>
#include <cassert>

namespace msdb
{
// plan
naive_filter_plan::naive_filter_plan()
{
}

const char* naive_filter_plan::name()
{
	return "naive_filter";
}

pAction naive_filter_plan::getAction()
{
	return std::make_shared<naive_filter_action>();
}

// pset
naive_filter_array_pset::naive_filter_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	// TODO::Check second param
}

pArrayDesc naive_filter_array_pset::inferSchema()
{
	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}