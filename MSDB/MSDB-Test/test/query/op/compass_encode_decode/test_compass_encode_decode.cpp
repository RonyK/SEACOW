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
namespace compass_encode_decode
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;
}

namespace data2D_star1024x1024
{
TEST(query_op_compass_encode_decode, star1024x1024_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, star1024x1024_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_compass_encode_decode, saturn1024x1024_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, saturn1024x1024_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_compass_encode_decode, solar1024x1024_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, solar1024x1024_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_compass_encode_decode, mars4096x2048_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, mars4096x2048_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_compass_encode_decode, mercurydem20480x10240_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, mercurydem20480x10240_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_compass_encode_decode, lunar102400x40960_ind_compass_encode_decode)
{
	test_qry_ind_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc,
												   numBins);
}

TEST(experiment_query_op_compass_encode_decode, lunar102400x40960_seq)
{
	test_body_seq_compass_encode_decode<value_type>(&getSourceArrayIfEmpty,
													&getSourceArrayDesc,
													numBins, compass_encode_decode::validation, compass_encode_decode::printFlag);
}
}		// data2D_lunar102400x40960
}		// caDummy
}		// core
}		// msdb