#include <pch.h>
#include <index/test_action_mmt.h>
#include <system/storageMgr.h>
#include <io/test_action_io.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_save_load, save_load_sc4x4)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_save_load, save_load_star1024x1024)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb