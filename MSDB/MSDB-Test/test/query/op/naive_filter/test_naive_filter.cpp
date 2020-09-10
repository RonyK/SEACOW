#include <pch.h>
#include <parse/predicate.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_naive_filter(_pFuncGetSourceArray_, pPredicate myPredicate, bool printFlag = false)
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

	auto filterArray = naive_filter(sourceArr, myPredicate);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		filterArray->print();
	}

	//EXPECT_TRUE(false);
	return filterArray;
}

namespace data2D_sc4x4
{
TEST(query_op_naive_filter, naive_filter_sc4x4)
{
	int64_t value = 2;
	bool printFlag = true;
	
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty, myPredicate, printFlag);

	equalTest<value_type>(outArr, value);
}		// TEST()
}		// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_naive_filter, naive_filter_star1024x1024)
{
	int64_t value = 5;
	bool printFlag = false;

	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = test_body_naive_filter<value_type>(&getSourceArrayIfEmpty, myPredicate);

	equalTest<value_type>(outArr, value);
}		// TEST()
}		// data2D_star1024x1024
}		// caDummy
}		// msdb