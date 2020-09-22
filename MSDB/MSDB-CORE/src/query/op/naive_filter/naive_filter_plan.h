#pragma once
#ifndef _MSDB_OP_NAIVE_FILTER_PLAN_H_
#define _MSDB_OP_NAIVE_FILTER_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class naive_filter_plan : public opPlan
{
public:
	naive_filter_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class naive_filter_array_pset : public opArrayParamSet
{
public: 
	naive_filter_array_pset(parameters& pSet);
};

class naive_filter_plan_pset : public opPlanParamSet
{
public:
	naive_filter_plan_pset(parameters& pSet);
};
}

#endif // _MSDB_OP_NAIVE_FILTER_PLAN_H_
