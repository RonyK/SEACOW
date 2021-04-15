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
TEST(query_op_save_load, star1024x1024_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_save_load, saturn1024x1024_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_save_load, solar1024x1024_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_save_load, mars4096x2048_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_save_load, mercurydem20480x10240_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_save_load, lunar102400x40960_save_load)
{
	test_body_save_load<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_lunar102400x40960
}	// caDummy
}	// msdb