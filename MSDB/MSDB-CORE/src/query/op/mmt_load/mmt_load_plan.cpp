#include <op/mmt_load/mmt_load_plan.h>
#include <op/mmt_load/mmt_load_action.h>
#include <cassert>

namespace msdb
{
mmt_load_plan::mmt_load_plan()
{
}
const char* mmt_load_plan::name()
{
	return "mmt_load_plan";
}

pAction mmt_load_plan::getAction()
{
	return std::make_shared<mmt_load_action>();
}
mmt_load_pset::mmt_load_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}
