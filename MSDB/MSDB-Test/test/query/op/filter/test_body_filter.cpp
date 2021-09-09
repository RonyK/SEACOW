#include <pch_test.h>
#include <query/op/filter/test_body_filter.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
pArray exe_act_ind_naive_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry)
{
	std::shared_ptr<naive_filter_plan> filterPlan;
	std::shared_ptr<naive_filter_action> filterAction;
	getNaiveFilter(sourceArr[0]->getDesc(), myPredicate, filterPlan, filterAction, qry);

	auto afterArray = filterAction->execute(sourceArr, qry);

	return afterArray;
}
pArray exe_act_ind_index_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry)
{
	std::shared_ptr<index_filter_plan> filterPlan;
	std::shared_ptr<index_filter_action> filterAction;
	getIndexFilter(sourceArr[0]->getDesc(), myPredicate, filterPlan, filterAction, qry);

	auto afterArray = filterAction->execute(sourceArr, qry);

	return afterArray;
}
pTerm getEqualTerm(int64_t value, attributeId attrId)
{
	pExpression lExp = std::make_shared<expressionAttributeId>(attrId);
	pExpression rExp = std::make_shared<expressionInteger>(value);

	return std::make_shared<term>(lExp, rExp, termType::EQUAL);
}
}		// caDummy
}		// core
}		// msdb