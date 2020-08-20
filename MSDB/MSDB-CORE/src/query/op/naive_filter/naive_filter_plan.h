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
	virtual pAction getAction() override;
};

class naive_filter_pset : public opParamSet
{
public: 
	naive_filter_pset();
	naive_filter_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif // _MSDB_OP_NAIVE_FILTER_PLAN_H_
