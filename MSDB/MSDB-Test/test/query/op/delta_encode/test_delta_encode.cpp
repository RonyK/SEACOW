#include <pch.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_test_mmt_delta_encode, mmt_sc4x4_delta_encode)
{
	auto arr = mmt_build();
	arr = mmt_delta_encode(std::vector<pArray>({ arr }));
}   // TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb