#include <op/load/load_plan.h>
#include <op/load/load_action.h>

namespace msdb
{
load_plan::load_plan()
{
}

load_plan::~load_plan()
{
}

const char* load_plan::name()
{
	return "load_plan";
}

pAction load_plan::getAction()
{
	return std::make_shared<load_action>();
}

load_pset::load_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}	// msdb