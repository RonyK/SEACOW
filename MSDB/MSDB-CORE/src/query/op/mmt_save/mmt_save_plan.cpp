#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_save/mmt_save_action.h>
#include <cassert>

namespace msdb
{
mmt_save_plan::mmt_save_plan()
{
}

const char* mmt_save_plan::name()
{
	return "mmt_save_plan";
}
pAction mmt_save_plan::getAction()
{
	return std::make_shared<mmt_save_action>();
}
mmt_save_pset::mmt_save_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}	// msdb
