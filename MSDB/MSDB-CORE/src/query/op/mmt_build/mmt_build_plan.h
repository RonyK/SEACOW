#pragma once
#ifndef _MSDB_MMT_BUILD_PLAN_H_
#define _MSDB_MMT_BUILD_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_build_plan : public opPlan
{
public:
	mmt_build_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_build_pset : public opArrayParamSet
{
public:
	mmt_build_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif		// _MSDB_MMT_BUILD_PLAN_H_