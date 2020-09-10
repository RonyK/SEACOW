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
}
}