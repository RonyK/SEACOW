#include <pch.h>
#include <parse/predicate.h>
#include <test/op/filter/testFilter.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_index_filter(_pFuncGetSourceArray_, pPredicate myPredicate, eleDefault mmtLevel, bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	auto outArr = mmt_build(sourceArr, mmtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	auto filteredArr = index_filter(std::vector<pArray>({ outArr }), myPredicate);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		filteredArr->print();
	}

	EXPECT_TRUE(false);
	return filteredArr;
}

namespace data2D_sc4x4
{
TEST(query_op_index_filter, index_filter_sc4x4)
{
	int64_t value = 2;
	bool printFlag = true;

	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_index_filter<value_type>(&getSourceArrayIfEmpty, myPredicate, mmtLevel, printFlag);

	equalTest<value_type>(outArr, value);
}		// TEST()
}		// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_index_filter, index_filter_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_index_filter<value_type>(&getSourceArrayIfEmpty, myPredicate, mmtLevel);

	equalTest<value_type>(outArr, value);
}		// TEST()
}		// data2D_star1024x1024
}		// caDummy
}		// msdb