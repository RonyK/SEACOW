#include <pch.h>
#include <query/op/filter/test_body_filter.h>

namespace msdb
{
namespace caDummy
{
int64_t numTests = 5;
int64_t value = 5;

// ##################################################
// # sc4x4
namespace data2D_sc4x4
{
TEST(query_op_index_filter, sc4x4_seq_load_naive_filter)
{
	bool saveArray = false;
	bool printFlag = true;
	bool validation = true;

	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()


TEST(query_op_index_filter, sc4x4_seq_load_index_filter)
{
	bool saveArray = false;
	bool printFlag = true;
	bool validation = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, sc4x4_seq_se_index_filter)
{
	bool saveArray = false;
	bool printFlag = true;
	bool validation = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, saveArray, validation, printFlag);
}	// TEST()
}	// data2D_sc4x4
// ##################################################

// ##################################################
// # star1024x1024
namespace data2D_star1024x1024
{
TEST(query_op_index_filter, star1024x1024_seq_load_naive_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_load_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_spiht_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_se_index_filter)
{
	bool saveArray = true;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_compass_index_filter)
{
	bool saveArray = true;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_compass_index_filter<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, numBins,
												   value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_zip_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_zip_index_filter<value_type>(&getSourceArrayIfEmpty,
											   &getSourceArrayDesc,
											   value, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_load_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_spiht_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, star1024x1024_seq_random_se_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 numTests, saveArray, validation, printFlag);
}	// TEST()
}	// data2D_star1024x1024
// ##################################################

// ##################################################
// # star1024x1024
namespace data2D_saturn1024x1024
{
TEST(query_op_index_filter, saturn1024x1024_seq_load_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_spiht_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_se_index_filter)
{
	bool saveArray = true;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_load_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_spiht_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, saturn1024x1024_seq_random_se_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 numTests, saveArray, validation, printFlag);
}	// TEST()
}	// data2D_saturn1024x1024
// ##################################################

// ##################################################
// # star1024x1024
namespace data2D_solar1024x1024
{
TEST(query_op_index_filter, solar1024x1024_seq_load_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_spiht_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_se_index_filter)
{
	bool saveArray = true;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_load_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_spiht_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, solar1024x1024_seq_random_se_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 numTests, saveArray, validation, printFlag);
}	// TEST()
}	// data2D_solar1024x1024
// ##################################################

// ##################################################
// # star1024x1024
namespace data2D_mars4096x2048
{
TEST(query_op_index_filter, mars4096x2048_seq_load_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc,
												value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_spiht_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel,
												 value, saveArray, validation, printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_se_index_filter)
{
	bool saveArray = true;
	bool printFlag = false;
	bool validation = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_load_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_load_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc,
													   numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_spiht_naive_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_spiht_random_naive_filter<value_type>(&getSourceArrayIfEmpty,
														&getSourceArrayDesc, wtLevel,
														numTests, saveArray, validation, printFlag);
}	// TEST()

TEST(experiment_query_op_index_filter, mars4096x2048_seq_random_se_index_filter)
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = false;

	test_body_seq_se_random_index_filter<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, wtLevel, mmtLevel,
													 numTests, saveArray, validation, printFlag);
}	// TEST()
}	// data2D_mars4096x2048
// ##################################################
}	// caDummy
}	// msdb