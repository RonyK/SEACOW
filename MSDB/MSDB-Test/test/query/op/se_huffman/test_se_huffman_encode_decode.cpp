#include <pch.h>
#include "qry_se_huffman_encode_decode.h"
#include <compression/fixedHuffmanCoder.h>

namespace msdb
{
namespace caDummy
{
namespace se_huffman_encode_decode
{
bool encodeArray = false;
bool printFlag = false;
bool validation = true;
}	// huffman_encode_decode

class query_op_se_huffman : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		pQuery qry = std::make_shared<query>();
		BOOST_LOG_TRIVIAL(debug) << "SetUp";
	}

	virtual void TearDown()
	{

	}
};

namespace data2D_star1024x1024
{
TEST(query_op_se_huffman, star1024x1024_ind_encode)
{
	test_qry_ind_se_huffman_encode<value_type>(&getSourceArrayIfEmpty,
											   wtLevel, mmtLevel,
											   se_huffman_encode_decode::printFlag);
}
TEST(query_op_se_huffman, star1024x1024_ind_decode)
{
	test_qry_ind_se_huffman_decode<value_type>(&getSourceArrayDesc,
											   wtLevel, mmtLevel,
											   se_huffman_encode_decode::printFlag);
}

TEST(query_op_se_huffman, star1024x1024_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, star1024x1024_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);

	auto coder1 = fixedHuffmanCoder<1, uint8_t>::instance();
	auto coder2 = fixedHuffmanCoder<2, uint8_t>::instance();
	auto coder3 = fixedHuffmanCoder<3, uint8_t>::instance();
	auto coder4 = fixedHuffmanCoder<4, uint8_t>::instance();
	auto coder5 = fixedHuffmanCoder<5, uint8_t>::instance();
	auto coder6 = fixedHuffmanCoder<6, uint8_t>::instance();
	auto coder7 = fixedHuffmanCoder<7, uint8_t>::instance();
	auto coder8 = fixedHuffmanCoder<8, uint8_t>::instance();

	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder1->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder2->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder3->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder4->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder5->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder6->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder7->printEncodeTable();
	BOOST_LOG_TRIVIAL(debug) << "=====";
	coder8->printEncodeTable();
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_se_huffman, saturn1024x1024_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, saturn1024x1024_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}
}

namespace data2D_solar1024x1024
{
TEST(query_op_se_huffman, solar1024x1024_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, solar1024x1024_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_se_huffman, mars4096x2048_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, mars4096x2048_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_se_huffman, mercurydem20480x10240_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, mercurydem20480x10240_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_se_huffman, lunar102400x40960_ind_encode_decode)
{
	test_body_ind_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_se_huffman, lunar102400x40960_seq_encode_decode)
{
	test_body_seq_se_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   wtLevel, mmtLevel,
													   se_huffman_encode_decode::validation, se_huffman_encode_decode::printFlag);
}
}	// data2D_lunar102400x40960
}	// caDummy
}	// msdb