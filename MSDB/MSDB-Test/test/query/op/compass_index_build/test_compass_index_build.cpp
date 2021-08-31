#include <pch.h>
#include <util/timer.h>
#include <compression/test_action_compression.h>
#include <compression/test_qry_compass.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_compass_index_build, star1024x1024_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_compass_index_build, saturn1024x1024_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_compass_index_build, solar1024x1024_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_compass_index_build, mars4096x2048_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_compass_index_build, mercurydem20480x10240_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_compass_index_build, lunar102400x40960_ind_compass_index_build)
{
	test_qry_ind_compass_index_build<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 numBins);
}
}		// data2D_lunar102400x40960
}		// caDummy
}		// core
}		// msdb