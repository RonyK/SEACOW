#include <pch.h>
#include <index/test_action_mmt.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_mmt_delta_encode(_pFuncGetSourceArray_, eleDefault mmtLevel)
{
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);

	auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	std::cout << "##############################" << std::endl;
	auto arr_mmt_delta_encode = exe_act_ind_mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
	std::cout << "##############################" << std::endl;

	//EXPECT_TRUE(false);
	return arr_mmt_delta_encode;
}

namespace data2D_sc4x4
{
TEST(query_op_test_mmt_delta_encode, mmt_delta_encode_sc4x4)
{
	auto output = test_body_mmt_delta_encode<value_type>(&getSourceArrayIfEmpty, mmtLevel);
	mmt_delta_encode_test(std::static_pointer_cast<mmt_delta_encode_array>(output));
}   // TEST()

}	// data2D_sc4x4
}	// caDummy
}	// msdb