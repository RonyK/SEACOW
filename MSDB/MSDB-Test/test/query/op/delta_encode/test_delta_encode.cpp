#include <pch.h>
#include <array/arrayMgr.h>
#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_test_mmt_delta_encode, mmt_sc4x4_delta_encode)
{
	// Should build mmt before
	std::vector<pArray> sourceArr = getSourceArray();
	std::shared_ptr<mmt_delta_encode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_encode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaEncode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

}   // TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb