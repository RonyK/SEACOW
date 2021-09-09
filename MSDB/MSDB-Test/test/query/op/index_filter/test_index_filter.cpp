#include <pch_test.h>
#include <query/op/filter/test_body_filter.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace index_filter
{
int64_t numTests = 3;
int64_t value = 5;

bool saveArray = false;
bool printFlag = false;
bool validation = true;
}

// ##################################################
// # sc4x4
namespace data2D_sc4x4
{
TEST(query_op_index_filter, sc4x4_seq_load_naive_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, sc4x4_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_sc4x4
// ##################################################

// ##################################################
// # star1024x1024
namespace data2D_star1024x1024
{
TEST(query_op_index_filter, star1024x1024_seq_load_naive_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_spiht_naive_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_compass_index_filter)
{
	test_body_seq_compass_index_filter<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, numBins,
												   index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_zip_naive_filter)
{
	test_body_seq_zip_naive_filter<value_type>(&getSourceArrayIfEmpty,
											   &getSourceArrayDesc,
											   index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_star1024x1024
// ##################################################

// ##################################################
// # saturn1024x1024
namespace data2D_saturn1024x1024
{
TEST(query_op_index_filter, saturn1024x1024_seq_load_index_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_spiht_index_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_saturn1024x1024
// ##################################################

// ##################################################
// # solar1024x1024
namespace data2D_solar1024x1024
{
TEST(query_op_index_filter, solar1024x1024_seq_load_index_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_spiht_index_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_solar1024x1024
// ##################################################

// ##################################################
// # data2D_mars4096x2048
namespace data2D_mars4096x2048
{
TEST(query_op_index_filter, mars4096x2048_seq_load_index_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_spiht_index_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_mars4096x2048
// ##################################################

// ##################################################
// # data2D_mercurydem20480x10240
namespace data2D_mercurydem20480x10240
{
TEST(query_op_index_filter, mercurydem20480x10240_seq_load_index_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, mercurydem20480x10240_seq_spiht_index_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, mercurydem20480x10240_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mercurydem20480x10240_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_mercurydem20480x10240
// ##################################################

// ##################################################
// # data2D_lunar102400x40960
namespace data2D_lunar102400x40960
{
TEST(query_op_index_filter, lunar102400x40960_seq_load_index_filter)
{
	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_spiht_index_filter)
{
	test_body_seq_spiht_naive_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_se_index_filter)
{
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  index_filter::value, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_load_naive_filter)
{
	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_spiht_naive_filter)
{
	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_zip_naive_filter)
{
	test_body_seq_zip_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(query_op_index_filter, lunar102400x40960_seq_random_compass_naive_filter)
{
	test_body_seq_compass_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_compass_index_filter)
{
	test_body_seq_compass_random_index_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc, numBins,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_se_naive_filter)
{
	test_body_seq_se_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_se_index_filter)
{
	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_se_huffman_index_filter)
{
	test_body_seq_se_huffman_random_index_filter<value_type>(&getSourceArrayIfEmpty,
															 &getSourceArrayDesc, wtLevel, mmtLevel,
															 index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_lzw_naive_filter)
{
	test_body_seq_lzw_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_huffman_naive_filter)
{
	test_body_seq_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														  &getSourceArrayDesc,
														  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_adapt_huffman_naive_filter)
{
	test_body_seq_adapt_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
																&getSourceArrayDesc,
																index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, lunar102400x40960_seq_random_lzw_huffman_naive_filter)
{
	test_body_seq_lzw_huffman_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
															  &getSourceArrayDesc,
															  index_filter::numTests, index_filter::saveArray, index_filter::validation, index_filter::printFlag);
}	// TEST()
}	// data2D_lunar102400x40960
// ##################################################
}	// caDummy
}	// core
}	// msdb