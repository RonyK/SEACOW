#include <pch.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
/// <summary>
/// Test Body for Index Filter
/// 
/// Steps of query are all indivisually executed.
/// </summary>
template <typename value_type>
pArray test_body_index_filter(_pFuncGetSourceArray_, eleDefault mmtLevel, int64_t value, bool printFlag)
{
	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = action_execute_index_filter<value_type>(getSourceArrayIfEmpty,
														  myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = action_execute_naive_filter<value_type>(getSourceArrayIfEmpty,
																myNaivePredicate, false);
	compArrary<value_type>(filterOutArr, outArr);
	//EXPECT_TRUE(false);
	return outArr;
}

/// <summary>
/// Test Body for Index Filter
/// 
/// Steps of query are all linked in a plan.
/// Test infering array schemas and bitmaps on the query steps.
/// </summary>
template <typename value_type>
pArray test_body_index_filter_seq(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  eleDefault mmtLevel, int64_t value, bool printFlag)
{
	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = action_execute_load_index_filter<value_type>(getSourceArrayIfEmpty,
															   myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = action_execute_naive_filter<value_type>(getSourceArrayIfEmpty,
																myNaivePredicate, false);
	compArrary<value_type>(filterOutArr, outArr);
	BOOST_LOG_TRIVIAL(debug) << "Array: " << outArr->getDesc()->name_;
	//EXPECT_TRUE(false);
	return outArr;
}

/// <summary>
/// Test Body for an Index Filter Operator for an array compressed in a Se_compression.
/// 
/// </summary>
template <typename value_type>
pArray test_body_index_filter_se_seq(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault wtLevel, eleDefault mmtLevel,
									 int64_t value, bool printFlag)
{
	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = action_execute_se_index_filter<value_type>(getSourceArrayIfEmpty,
															 myPredicate, wtLevel, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	//pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	//auto filterOutArr = action_execute_naive_filter<value_type>(getSourceArrayIfEmpty,
	//															myNaivePredicate, false);

	//auto naiveBitmap = filterOutArr->getChunkBitmap();
	//BOOST_LOG_TRIVIAL(debug) << "naiveBitmap";
	//naiveBitmap->print();
	//auto seBitmap = outArr->getChunkBitmap();
	//BOOST_LOG_TRIVIAL(debug) << "seBitmap";
	//seBitmap->print();
	//compArrary<value_type>(filterOutArr, outArr);
	
	BOOST_LOG_TRIVIAL(debug) << "Array: " << outArr->getDesc()->name_;

	//EXPECT_TRUE(false);
	return outArr;
}

/// <summary>
/// Test Body for an Index Filter Operator for an array compressed in a SPIHT.
/// 
/// </summary>
template <typename value_type>
pArray test_body_index_filter_SPIHT_seq(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault wtLevel, eleDefault mmtLevel,
										int64_t value, bool printFlag)
{
	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = action_execute_spiht_index_filter<value_type>(getSourceArrayIfEmpty,
															 myPredicate, wtLevel, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	//pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	//auto filterOutArr = action_execute_naive_filter<value_type>(getSourceArrayIfEmpty,
	//															myNaivePredicate, false);
	//compArrary<value_type>(filterOutArr, outArr);

	//EXPECT_TRUE(false);
	return outArr;
}

namespace data2D_sc4x4
{
TEST(query_op_index_filter, index_filter_sc4x4)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_index_filter<value_type>(&getSourceArrayIfEmpty,
									   mmtLevel, value, printFlag);
}	// TEST()


TEST(query_op_index_filter, index_filter_seq_sc4x4)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_index_filter_seq<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, mmtLevel,
										   value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_se_seq_sc4x4)
{
	int64_t value = 0;
	bool printFlag = true;

	test_body_index_filter_se_seq<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_index_filter, index_filter_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter<value_type>(&getSourceArrayIfEmpty,
									   mmtLevel, value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_seq<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, mmtLevel,
										   value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_spiht_seq_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_SPIHT_seq<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_se_seq_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_se_seq<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_index_filter, index_filter_seq_saturn1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_seq<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, mmtLevel,
										   value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_spiht_seq_saturn1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_SPIHT_seq<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_se_seq_saturn1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_se_seq<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_index_filter, index_filter_seq_solar1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_seq<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, mmtLevel,
										   value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_spiht_seq_solar1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_SPIHT_seq<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_se_seq_solar1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;
	test_body_index_filter_se_seq<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												value, printFlag);
}	// TEST()
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_index_filter, index_filter_seq_mars4096x2048)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_seq<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, mmtLevel,
										   value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_spiht_seq_mars4096x2048)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_SPIHT_seq<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, wtLevel, mmtLevel,
												 value, printFlag);
}	// TEST()

TEST(query_op_index_filter, index_filter_seq_se_seq_mars4096x2048)
{
	int64_t value = 5;
	bool printFlag = false;

	test_body_index_filter_se_seq<value_type>(&getSourceArrayIfEmpty,
											  &getSourceArrayDesc, wtLevel, mmtLevel,
											  value, printFlag);
}	// TEST()
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb