#include <pch.h>
#include <util/timer.h>
#include <compression/test_action_compression.h>
#include <compression/test_qry_compass.h>

namespace msdb
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
}	// caDummy
}	// msdb