#include <pch.h>
#include "qry_lzw_huffman_encode_decode.h"

namespace msdb
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
}
TEST(query_op_lzw_huffman, star1024x1024_ind_decode)
{
	test_qry_ind_lzw_huffman_decode<value_type>(&getSourceArrayDesc,
												lzw_huffman_encode_decode::printFlag);
}

TEST(query_op_lzw_huffman, star1024x1024_ind_encode_decode)
{
	test_qry_ind_lzw_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty, 
													   &getSourceArrayDesc,
													   lzw_huffman_encode_decode::validation, lzw_huffman_encode_decode::printFlag);
}
}	// data2D_star1024x1024
}	// caDummy
}	// msdbz