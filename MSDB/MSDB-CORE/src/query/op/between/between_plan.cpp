#include <stdafx.h>
#include <op/between/between_plan.h>
#include <op/between/between_action.h>

namespace msdb
{
	// plan
	between_plan::between_plan()
	{
	}

	const char* between_plan::name()
	{
		return "between_plan";
	}

	pAction between_plan::makeAction()
	{
		return std::make_shared<between_action>();
	}

	// pset
	between_array_pset::between_array_pset(parameters& pSet)
		: opArrayParamSet(pSet)
	{
		assert(this->params_.size() == 3);
		assert(this->params_[1]->type() == opParamType::COOR);
		assert(this->params_[2]->type() == opParamType::COOR);
	}

	pArrayDesc between_array_pset::inferSchema()
	{
		auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
		auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

		return aInferDesc;
	}
}
