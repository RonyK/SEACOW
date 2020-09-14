#include <pch.h>

namespace msdb
{
namespace caDummy
{
//std::shared_ptr<load_plan> getLoadPlan(pArrayDesc sourceArrDesc, pQuery qry)
//{
//	return std::get<0>(
//		getSinglePlanParamPlan<load_plan, load_array_pset>(sourceArrDesc, qry));
//}
std::shared_ptr<naive_filter_plan> getNaiveFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<naive_filter_plan, naive_filter_plan_pset>(sourcPlan, inPredicate, qry);
}
std::shared_ptr<index_filter_plan> getIndexFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<index_filter_plan, index_filter_plan_pset>(sourcPlan, inPredicate, qry);
}
}	// caDummy
}	// msdb