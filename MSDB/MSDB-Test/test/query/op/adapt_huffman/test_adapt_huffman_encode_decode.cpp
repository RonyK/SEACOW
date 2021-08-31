#include <pch.h>
#include "qry_adapt_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace adapt_huffman_encode_decode
{
bool encodeArray = false;
bool printFlag = false;
bool validation = true;
}	// huffman_encode_decode

namespace data2D_star1024x1024
{
TEST(query_op_adapt_huffman, star1024x1024_ind_encode)
{
	test_qry_ind_adapt_huffman_encode<value_type>(&getSourceArrayIfEmpty,
												  adapt_huffman_encode_decode::printFlag);
}
TEST(query_op_adapt_huffman, star1024x1024_ind_decode)
{
	test_qry_ind_adapt_huffman_decode<value_type>(&getSourceArrayDesc,
												  adapt_huffman_encode_decode::printFlag);
}

TEST(query_op_adapt_huffman, star1024x1024_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, star1024x1024_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_adapt_huffman, saturn1024x1024_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, saturn1024x1024_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}

namespace data2D_solar1024x1024
{
TEST(query_op_adapt_huffman, solar1024x1024_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, solar1024x1024_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_adapt_huffman, mars4096x2048_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, mars4096x2048_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_adapt_huffman, mercurydem20480x10240_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, mercurydem20480x10240_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(query_op_adapt_huffman, lunar102400x40960_ind_encode_decode)
{
	test_body_ind_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}

TEST(experiment_query_op_adapt_huffman, lunar102400x40960_seq_encode_decode)
{
	test_body_seq_adapt_huffman_encode_decode<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  adapt_huffman_encode_decode::validation, adapt_huffman_encode_decode::printFlag);
}
}	// data2D_lunar102400x40960
}	// caDummy
}	// core
}	// msdb