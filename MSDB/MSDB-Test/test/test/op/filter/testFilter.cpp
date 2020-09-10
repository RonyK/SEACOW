#include <pch.h>
#include <test/op/filter/testFilter.h>

namespace msdb
{
namespace caDummy
{
pArray naive_filter(_vectorSourceArray_, pPredicate myPredicate)
{
	std::shared_ptr<naive_filter_plan> filterPlan;
	std::shared_ptr<naive_filter_action> filterAction;
	pQuery filterQuery;
	getNaiveFilter(sourceArr[0]->getDesc(), myPredicate, filterPlan, filterAction, filterQuery);

	auto afterArray = filterAction->execute(sourceArr, filterQuery);

	return afterArray;
}
pArray index_filter(_vectorSourceArray_, pPredicate myPredicate)
{
	std::shared_ptr<index_filter_plan> filterPlan;
	std::shared_ptr<index_filter_action> filterAction;
	pQuery filterQuery;
	getIndexFilter(sourceArr[0]->getDesc(), myPredicate, filterPlan, filterAction, filterQuery);

	auto afterArray = filterAction->execute(sourceArr, filterQuery);

	return afterArray;
}
pTerm getEqualTerm(int64_t value, attributeId attrId)
{
	pExpression lExp = std::make_shared<expressionAttributeId>(attrId);
	pExpression rExp = std::make_shared<expressionInteger>(value);

	return std::make_shared<term>(lExp, rExp, termType::EQUAL);
}
}	// caDummy
}	// msdb