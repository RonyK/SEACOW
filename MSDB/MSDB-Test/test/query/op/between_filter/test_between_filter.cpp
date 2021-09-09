#include <pch_test.h>
#include "qry_between_filter.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace between_filter
{
int64_t numTests = 3;
std::vector<float> selectivities = { 0.01, 0.1, 1, 10 };
//std::vector<float> selectivities = { 10 };

bool saveArray = false;
bool printFlag = false;
bool validation = true;
}
TEST(io_test, clear_cache)
{
	system("EmptyStandbyList.exe");
}	// TEST()

// ##################################################
// # star1024x1024
namespace data2D_star1024x1024
{
TEST(experiment_query_op_between_filter, star1024x1024_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, star1024x1024_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_star1024x1024
// ##################################################
// ##################################################
// # saturn1024x1024
namespace data2D_saturn1024x1024
{
TEST(experiment_query_op_between_filter, saturn1024x1024_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, saturn1024x1024_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_saturn1024x1024
// ##################################################
// ##################################################
// # solar1024x1024
namespace data2D_solar1024x1024
{
TEST(experiment_query_op_between_filter, solar1024x1024_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, solar1024x1024_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_solar1024x1024
// ##################################################
// ##################################################
// # mars4096x2048
namespace data2D_mars4096x2048
{
TEST(experiment_query_op_between_filter, mars4096x2048_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mars4096x2048_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_mars4096x2048
// ##################################################
// ##################################################
// # mercurydem20480x10240
namespace data2D_mercurydem20480x10240
{
TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, mercurydem20480x10240_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_mercurydem20480x10240
// ##################################################
// ##################################################
// # lunar102400x40960
namespace data2D_lunar102400x40960
{
TEST(experiment_query_op_between_filter, lunar102400x40960_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests,
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_spiht_naive_filter)
{
	test_body_seq_spiht_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  wtLevel,
														  between_filter::numTests,
														  between_filter::selectivities, dimX, dimY,
														  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_zip_naive_filter)
{
	test_body_seq_zip_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_compass_naive_filter)
{
	test_body_seq_compass_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_compass_index_filter)
{
	test_body_seq_compass_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  numBins,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_se_naive_filter)
{
	test_body_seq_se_naive_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_se_index_filter)
{
	test_body_seq_se_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc,
															 wtLevel, mmtLevel,
															 between_filter::numTests,
															 between_filter::selectivities, dimX, dimY,
															 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_se_huffman_index_filter)
{
	test_body_seq_se_huffman_index_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																	 &getSourceArrayDesc,
																	 wtLevel, mmtLevel,
																	 between_filter::numTests,
																	 between_filter::selectivities, dimX, dimY,
																	 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_lzw_naive_filter)
{
	test_body_seq_lzw_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc,
														between_filter::numTests,
														between_filter::selectivities, dimX, dimY,
														between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_huffman_naive_filter)
{
	test_body_seq_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
															&getSourceArrayDesc,
															between_filter::numTests,
															between_filter::selectivities, dimX, dimY,
															between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																  &getSourceArrayDesc,
																  between_filter::numTests,
																  between_filter::selectivities, dimX, dimY,
																  between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()

TEST(experiment_query_op_between_filter, lunar102400x40960_seq_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_between_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																between_filter::numTests,
																between_filter::selectivities, dimX, dimY,
																between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_lunar102400x40960
// ##################################################
}	// caDummy
}	// core
}	// msdb