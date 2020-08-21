#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/spiht_decode/spiht_decode_action.h>
#include <cassert>

namespace msdb
{
// plan
spiht_decode_plan::spiht_decode_plan()
{
}

const char* spiht_decode_plan::name()
{
	return "spiht_decode_plan";
}

pAction spiht_decode_plan::getAction()
{
	return std::make_shared<spiht_decode_action>();
}

// pset
spiht_decode_array_pset::spiht_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}