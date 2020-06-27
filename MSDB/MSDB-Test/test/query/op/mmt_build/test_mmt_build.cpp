#include <pch.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(mmt_build_action, mmt_serialize_sc8x8)
{
	std::vector<pArray> sourceArr = getSourceArray();

	eleDefault level = 1;
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);
}
}	// data2D_sc8x8
}	// caDummy
}	// msdb