#pragma once
#ifndef _MSDB_OP_NAIVE_FILTER_PLAN_H_
#define _MSDB_OP_NAIVE_FILTER_PLAN_H_

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

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif // _MSDB_OP_NAIVE_FILTER_PLAN_H_
