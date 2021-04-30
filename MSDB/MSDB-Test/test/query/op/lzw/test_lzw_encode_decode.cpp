#include <pch.h>
#include <compression/test_qry_lzw.h>

namespace msdb
{
namespace caDummy
{
namespace lzw_encode_decode
{
bool encodeArray = false;
bool printFlag = false;
bool validation = true;
}

namespace data2D_star1024x1024
{
TEST(query_op_lzw, star1024x1024_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, star1024x1024_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag);
}

TEST(experiment_query_op_lzw_encode_decode, star1024x1024_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_lzw, saturn1024x1024_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, saturn1024x1024_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag);
}

TEST(experiment_query_op_lzw_encode_decode, saturn1024x1024_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_lzw, solar1024x1024_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, solar1024x1024_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag);
}

TEST(experiment_query_op_lzw_encode_decode, solar1024x1024_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_lzw, mars4096x2048_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, mars4096x2048_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag);
}

TEST(experiment_query_op_lzw_encode_decode, mars4096x2048_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_lzw, mercurydem20480x10240_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, mercurydem20480x10240_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag, lzw_encode_decode::validation);
}

TEST(experiment_query_op_lzw_encode_decode, mercurydem20480x10240_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_lzw, lunar102400x40960_ind_lzw_encode)
{
	test_qry_ind_lzw_encode<value_type>(&getSourceArrayIfEmpty,
										&getSourceArrayDesc);
}

TEST(query_op_lzw_decode, lunar102400x40960_seq_lzw_decode)
{
	test_body_seq_lzw_decode<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc,
										 lzw_encode_decode::printFlag);
}

TEST(experiment_query_op_lzw_encode_decode, lunar102400x40960_seq)
{
	test_body_seq_lzw_encode_decode<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												lzw_encode_decode::validation, lzw_encode_decode::printFlag);
}
}	// data2D_lunar102400x40960
}	// caDummy
}	// msdb