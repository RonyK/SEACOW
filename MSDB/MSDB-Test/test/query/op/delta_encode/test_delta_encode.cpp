#include <pch.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_test_mmt_delta_encode, mmt_delta_encode_sc4x4)
{
	auto arr_1 = mmt_build();
	auto arr_2 = mmt_delta_encode(std::vector<pArray>({ arr_1 }));
	mmt_delta_encode_test(std::static_pointer_cast<mmt_delta_encode_array>(arr_2));
}   // TEST()

}	// data2D_sc4x4
}	// caDummy
}	// msdb