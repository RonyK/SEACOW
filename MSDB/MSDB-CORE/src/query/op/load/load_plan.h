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
	virtual pAction getAction() override;
};

class load_pset : public opParamSet
{
public:
	load_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}	// msdb
#endif		// _MSDB_OP_LOAD_PLAN_H_