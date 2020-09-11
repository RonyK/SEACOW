#include <pch.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
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