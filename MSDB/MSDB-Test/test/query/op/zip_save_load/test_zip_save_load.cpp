#include <pch.h>
#include <compression/test_qry_zip.h>

namespace msdb
{
namespace caDummy
{
namespace zip_save_load
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;
}

namespace data2D_star1024x1024
{
TEST(query_op_zip, star1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, star1024x1024_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag);
}

TEST(experiment_query_op_zip_save_load, star1024x1024_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_zip, saturn1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, saturn1024x1024_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag);
}

TEST(experiment_query_op_zip_save_load, saturn1024x1024_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_zip, solar1024x1024_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, solar1024x1024_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag);
}

TEST(experiment_query_op_zip_save_load, solar1024x1024_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_zip, mars4096x2048_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, mars4096x2048_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag);
}

TEST(experiment_query_op_zip_save_load, mars4096x2048_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_zip, mercurydem20480x10240_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, mercurydem20480x10240_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag, zip_save_load::validation);
}

TEST(experiment_query_op_zip_save_load, mercurydem20480x10240_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_zip, lunar102400x40960_ind_zip_save)
{
	test_qry_ind_zip_save<value_type>(&getSourceArrayIfEmpty,
									  &getSourceArrayDesc);
}

TEST(query_op_zip_load, lunar102400x40960_seq_zip_load)
{
	test_body_seq_zip_load<value_type>(&getSourceArrayIfEmpty,
									   &getSourceArrayDesc,
									   zip_save_load::printFlag);
}

TEST(experiment_query_op_zip_save_load, lunar102400x40960_seq)
{
	test_body_seq_zip_save_load<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc,
											zip_save_load::validation, zip_save_load::printFlag);
}
}	// data2D_lunar102400x40960
}	// caDummy
}	// msdb