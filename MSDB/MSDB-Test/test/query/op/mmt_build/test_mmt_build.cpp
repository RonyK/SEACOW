#include <pch.h>
#include <index/test_action_mmt.h>

#include <array/arrayMgr.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_mmt_build(_pFuncGetSourceArray_, eleDefault mmtLevel)
{
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);

	auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);

	//EXPECT_TRUE(false);
	return arr_mmt_build;
}

template <typename value_type>
void print_mmt(pArray arr, attributeId attrId)
{
	auto attrIndex = arrayMgr::instance()->getAttributeIndex(arr->getId(), attrId);
	auto myMMT = std::static_pointer_cast<MinMaxTreeImpl<position_t, value_type>>(attrIndex);
	myMMT->print();
}

namespace data2D_sc4x4
{
TEST(query_op_mmt_build, mmt_build_sc4x4)
{
	auto output = test_body_mmt_build<value_type>(&getSourceArrayIfEmpty, mmtLevel);
	mmt_build_test(output);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_mmt_build, mmt_build_data2D_star1024x1024)
{
	auto output = test_body_mmt_build<value_type>(&getSourceArrayIfEmpty, mmtLevel);

	print_mmt<value_type>(output, 0);
}	// TEST()
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_mmt_build, mmt_build_data2D_saturn1024x1024)
{
	auto output = test_body_mmt_build<value_type>(&getSourceArrayIfEmpty, mmtLevel);

	print_mmt<value_type>(output, 0);
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_mmt_build, mmt_build_data2D_mars4096x2048)
{
	auto output = test_body_mmt_build<value_type>(&getSourceArrayIfEmpty, mmtLevel);

	//output->print();
	//print_mmt<value_type>(output, 0);
}	// TEST()
}	// mars4096x2048
}	// caDummy
}	// msdb