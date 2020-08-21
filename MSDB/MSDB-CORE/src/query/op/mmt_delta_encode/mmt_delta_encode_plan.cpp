#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>

namespace msdb
{
mmt_delta_encode_plan::mmt_delta_encode_plan()
{
}

mmt_delta_encode_plan::~mmt_delta_encode_plan()
{
}

const char* mmt_delta_encode_plan::name()
{
	return "mmt_delta_encode_plan";
}

pAction mmt_delta_encode_plan::getAction()
{
	return std::make_shared<mmt_delta_encode_action>();
}

mmt_delta_encode_pset::mmt_delta_encode_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}	// msdb