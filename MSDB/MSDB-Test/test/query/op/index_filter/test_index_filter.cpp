#include <pch.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_index_filter, index_filter_sc4x4)
{
	int64_t value = 0;
	bool printFlag = true;

	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_index_filter<value_type>(&getSourceArrayIfEmpty,
													 myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty,
														   myNaivePredicate, printFlag);
	compArrary<value_type>(filterOutArr, outArr);
	//EXPECT_TRUE(false);
}		// TEST()

TEST(query_op_index_filter, load_index_filter_sc4x4)
{
	int64_t value = 0;
	bool printFlag = true;

	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_load_index_filter<value_type>(&getSourceArrayIfEmpty,
														  myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty,
														   myNaivePredicate, printFlag);
	compArrary<value_type>(filterOutArr, outArr);
	//EXPECT_TRUE(false);
}		// TEST()
}		// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_index_filter, index_filter_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_index_filter<value_type>(&getSourceArrayIfEmpty,
													 myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty,
														   myNaivePredicate, printFlag);
	compArrary<value_type>(filterOutArr, outArr);
	//EXPECT_TRUE(false);
}		// TEST()

TEST(query_op_index_filter, load_index_filter_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	//////////////////////////////
	// Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_load_index_filter<value_type>(&getSourceArrayIfEmpty,
														  myPredicate, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// Naive filter array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty,
														   myNaivePredicate, printFlag);
	compArrary<value_type>(filterOutArr, outArr);
}		// TEST()
}		// data2D_star1024x1024
}		// caDummy
}		// msdb