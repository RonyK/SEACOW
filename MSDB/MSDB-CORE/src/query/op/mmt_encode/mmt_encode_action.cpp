#include <array/arrayMgr.h>
#include <op/mmt_encode/mmt_encode_action.h>
#include <index/mmt.h>


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
	assert(inputArrays.size() == 1);

	pArray arr = inputArrays[0];
	

	return pArray();
}

const char* mmt_encode_action::name()
{
	return "mmt_encode_action";
}

}
