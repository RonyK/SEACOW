#include <pch.h>
#include <index/test_action_mmt.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_mmt_delta_decode, mmt_delta_decode_sc4x4)
{
	//auto arr_encode = get_mmt_delta_encode_array();
	//auto arr_decode = exe_act_ind_mmt_delta_decode(std::vector<pArray>({ arr_encode }));

    std::vector<pArray> sourceArr;
    getSourceArrayIfEmpty(sourceArr);
    sourceArr[0]->setId(sourceArr[0]->getId());     // 441

    auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);
    std::cout << "##############################" << std::endl;
    std::cout << "Source Arr" << std::endl;
    arr_mmt_build->print();

    auto arr_delta_encode = exe_act_ind_mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
    std::cout << "##############################" << std::endl;
    std::cout << "Delta Arr" << std::endl;
    arr_delta_encode->print();

    auto arr_delta_decode = exe_act_ind_mmt_delta_decode(std::vector<pArray>({ arr_delta_encode }));
    std::cout << "##############################" << std::endl;
    std::cout << "Delta Decode Arr" << std::endl;
    arr_delta_decode->print();

	mmt_delta_decode_test(std::static_pointer_cast<mmt_delta_decode_array>(arr_delta_decode));
}       // TEST()
}	    // data2D_sc4x4
}	    // caDummy
}		// core
}		// msdb