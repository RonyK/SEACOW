#include <pch.h>
#include <compression/test_qry_zip.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_zip_save, star1024x1024_ind_compass_encode_decode)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}
}	// data2D_star1024x1024
}	// caDummy
}	// msdb