#include <pch.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_mmt_delta_decode, mmt_delta_decode_sc4x4)
{
	auto arr_encode = get_mmt_delta_encode_array();
	auto arr_decode = mmt_delta_decode(std::vector<std::shared_ptr<mmt_delta_encode_array>>({ arr_encode }));
	mmt_delta_decode_test(std::static_pointer_cast<mmt_delta_decode_array>(arr_decode));
}   // TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb