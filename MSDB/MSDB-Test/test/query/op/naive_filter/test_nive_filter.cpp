#include <pch.h>
#include <parse/predicate.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
pTerm getEqualTerm(int64_t value, attributeId attrId = 0)
{
	pExpression lExp = std::make_shared<expressionAttributeId>(attrId);
	pExpression rExp = std::make_shared<expressionInteger>(value);
	
	return std::make_shared<term>(lExp, rExp, termType::EQUAL);
}

template <typename value_type>
void test_body_naive_filter(_pFuncGetSourceArray_, pPredicate myPredicate, bool printFlag = false)
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

	EXPECT_TRUE(false);
}

namespace data2D_sc4x4
{
TEST(query_op_naive_filter, naive_filter_sc4x4)
{
	bool printFlag = true;
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(2));
	test_body_naive_filter<value_type>(&getSourceArrayIfEmpty, myPredicate, printFlag);
}		// TEST()
}		// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_naive_filter, naive_filter_star1024x1024)
{
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(5));
	test_body_naive_filter<value_type>(&getSourceArrayIfEmpty, myPredicate);
}		// TEST()
}		// data2D_star1024x1024
}		// caDummy
}		// msdb