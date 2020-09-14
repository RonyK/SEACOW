#pragma once
#ifndef _MSDB_OP_LOAD_PLAN_H_
#define _MSDB_OP_LOAD_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class load_plan : public opPlan
{
public:
	load_plan();
	~load_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class load_array_pset : public opArrayParamSet
{
public:
	load_array_pset(parameters& pSet);
};

//class load_plan_pset : public opPlanParamSet
//{
//public:
//	load_plan_pset(parameters& pSet);
//};
}	// msdb
#endif		// _MSDB_OP_LOAD_PLAN_H_