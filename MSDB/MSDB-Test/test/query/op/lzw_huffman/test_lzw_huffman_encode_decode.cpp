#include <pch_test.h>
#include "qry_lzw_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace lzw_huffman_encode_decode
{
bool encodeArray = false;
bool printFlag = false;
bool validation = true;
}	// lzw_huffman_encode_decode

namespace data2D_star1024x1024
{
TEST(query_op_lzw_huffman, star1024x1024_ind_encode)
{
	test_qry_ind_lzw_huffman_encode<value_type>(&getSourceArrayIfEmpty,
												lzw_huffman_encode_decode::printFlag);
}	// TEST
TEST(query_op_lzw_huffman, star1024x1024_ind_decode)
{
	test_qry_ind_lzw_huffman_decode<value_type>(&getSourceArrayDesc,
												lzw_huffman_encode_decode::printFlag);
}	// TEST

TEST(query_op_lzw_huffman, star1024x1024_ind_encode_decode)
{
	test_qry_ind_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty, 
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST

TEST(experiment_query_op_lzw_huffman, star1024x1024_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(experiment_query_op_lzw_huffman, saturn1024x1024_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(experiment_query_op_lzw_huffman, solar1024x1024_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(experiment_query_op_lzw_huffman, mars4096x2048_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST
}	// data2D_solar1024x1024

namespace data2D_mercurydem20480x10240
{
TEST(experiment_query_op_lzw_huffman, mercurydem20480x10240_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}	// TEST
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(experiment_query_op_lzw_huffman, lunar102400x40960_seq_encode_decode)
{
	test_qry_seq_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}		// TEST
}		//data2D_lunar102400x40960
}		// caDummy
}		// core
}		// msdb