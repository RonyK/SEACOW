#pragma once
#ifndef _MSDB_OP_MMT_LOAD_PLAN_H_
#define _MSDB_OP_MMT_LOAD_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_load_plan : public opPlan
{
public:
	mmt_load_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_load_pset : public opParamSet
{
public:
	mmt_load_pset();
	mmt_load_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}	// msdb

#endif	// _MSDB_OP_MMT_LOAD_PLAN_H_