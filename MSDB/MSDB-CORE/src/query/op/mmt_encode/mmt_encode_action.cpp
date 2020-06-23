#include <op/mmt_encode/mmt_encode_action.h>

namespace msdb
{
mmt_encode_action::mmt_encode_action()
{
}

mmt_encode_action::~mmt_encode_action()
{
}

pArray mmt_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	return pArray();
}

const char* mmt_encode_action::name()
{
	return "mmt_encode_action";
}

}
