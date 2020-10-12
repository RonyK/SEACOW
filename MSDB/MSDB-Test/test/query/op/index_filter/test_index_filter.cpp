#include <pch.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_index_filter, sc4x4_seq_load_naive_filter)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												value, printFlag);
}	// TEST()


TEST(query_op_index_filter, sc4x4_seq_load_index_filter)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, mmtLevel,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, sc4x4_seq_se_index_filter)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_index_filter, star1024x1024_seq_load_naive_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_load_naive_filter<value_type>(&getSourceArrayIfEmpty,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_load_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, mmtLevel,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_spiht_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, star1024x1024_seq_se_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_index_filter, saturn1024x1024_seq_load_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, mmtLevel,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_spiht_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, saturn1024x1024_seq_se_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_index_filter, solar1024x1024_seq_load_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, mmtLevel,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_spiht_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, solar1024x1024_seq_se_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;
	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_index_filter, mars4096x2048_seq_load_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_load_index_filter<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, mmtLevel,
												value, printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_spiht_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_spiht_index_filter<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, mars4096x2048_seq_se_index_filter)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_seq_se_index_filter<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb