#include <pch.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_mmt_delta_decode, mmt_sc4x4_delta_decode)
{
	auto arr = get_mmt_delta_encode_array();
	std::vector<std::shared_ptr<mmt_delta_encode_array>> arrList({ arr });
	auto afterArray = mmt_delta_decode(arrList);

	std::cout << "mmt_delta_decode" << std::endl;
}   // TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb