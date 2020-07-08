#pragma once
#ifndef _MSDB_OP_SAVE_PLAN_H_
#define _MSDB_OP_SAVE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class save_plan : public opPlan
{
public:
	save_plan();
	~save_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class save_pset : public opParamSet
{
public:
	save_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}	// msdb
#endif	// _MSDB_OP_SAVE_PLAN_H_