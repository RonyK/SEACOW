#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <cassert>

namespace msdb
{
// plan
spiht_encode_plan::spiht_encode_plan()
{
}

const char* spiht_encode_plan::name()
{
	return "spiht_encode_plan";
}

pAction spiht_encode_plan::getAction()
{
	return std::make_shared<spiht_encode_action>(); 
}

// pset
spiht_encode_pset::spiht_encode_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}