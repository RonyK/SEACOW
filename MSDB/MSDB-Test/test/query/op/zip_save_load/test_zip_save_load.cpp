#include <pch.h>
#include <compression/test_qry_zip.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_zip, star1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip, star1024x1024_ind_zip_save_load)
{
	test_qry_ind_zip_save_load<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_zip, saturn1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_zip, solar1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_zip, mars4096x2048_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb