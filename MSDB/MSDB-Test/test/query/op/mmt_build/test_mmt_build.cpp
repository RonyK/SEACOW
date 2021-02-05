#include <pch.h>
#include <index/test_action_mmt.h>

#include <array/arrayMgr.h>
#include <index/test_qry_mmt.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

namespace msdb
{
namespace caDummy
{
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