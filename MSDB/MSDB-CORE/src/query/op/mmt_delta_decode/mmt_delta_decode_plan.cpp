#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

namespace msdb
{
mmt_delta_decode_plan::mmt_delta_decode_plan()
{
}

mmt_delta_decode_plan::~mmt_delta_decode_plan()
{
}

const char* mmt_delta_decode_plan::name()
{
	return "mmt_delta_decode";
}

pAction mmt_delta_decode_plan::getAction()
{
	return pAction();
}

mmt_delta_decode_pset::mmt_delta_decode_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}
