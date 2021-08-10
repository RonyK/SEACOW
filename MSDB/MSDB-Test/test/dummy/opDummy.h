#pragma once
#ifndef _MSDB_OPDUMMY_H_
#define _MSDB_OPDUMMY_H_

#include <pch.h>
#include <query/query.h>
#include <parse/predicate.h>
#include <query/opPlan.h>

namespace msdb
{
namespace caDummy
{
void tearDownQuery(pQuery qry, size_t expId = 0, size_t expTrial = 0, size_t dataId = 0, size_t methodId = 0);

template <typename value_type>
std::vector<pArray> getArrayFromFunction(_pFuncGetSourceArray_, bool printFlag = false)
{
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	return sourceArr;
}

template <typename value_type>
std::vector<pArray> getArrayDeskFromFunction(_pFuncGetSourceArrayDesc_, bool printFlag = false)
{
	std::vector<pArray> sourceArray;
	getSourceArrayDesc(sourceArr);
	return sourceArray;
}

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getSingleArrayParamOperator(pArrayDesc sourceArrDesc, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};

	return getOperator<plan_, action_, pset_>(params, myQuery);
}

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getLevelArrayParamOperator(pArrayDesc sourceArrDesc, eleDefault& level, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};

	return getOperator<plan_, action_, pset_>(params, myQuery);
}

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getPredicateArrayParamOperator(pArrayDesc sourceArrDesc, pPredicate myPredicate, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamPredicate>(myPredicate)
	};

	return getOperator<plan_, action_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getSingleArrayParamPlan(pArrayDesc sourceArrDesc, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getLevelArrayParamPlan(pArrayDesc sourceArrDesc, eleDefault& level, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getPredicateArrayParamPlan(pArrayDesc sourceArrDesc, pPredicate myPredicate, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamPredicate>(myPredicate)
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getSinglePlanParamPlan(pPlan sourcePlan, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamPlan>(sourcePlan)
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getLevelPlanParamPlan(pPlan sourcePlan, eleDefault& level, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamPlan>(sourcePlan),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getPredicatePlanParamPlan(pPlan sourcePlan, pPredicate myPredicate, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamPlan>(sourcePlan),
		std::make_shared<opParamPredicate>(myPredicate)
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getPlan_setPlanRange(pPlan sourcePlan, coor sp, coor ep, pQuery myQuery = nullptr)
{
	parameters params = {
		std::make_shared<opParamPlan>(sourcePlan),
		std::make_shared<opParamCoor>(std::make_shared<coor>(sp)),
		std::make_shared<opParamCoor>(std::make_shared<coor>(ep))
	};

	return getOpPlan<plan_, pset_>(params, myQuery);
}
}		// caDummy
}		// msdb
#endif	// _MSDB_OPDUMMY_H_