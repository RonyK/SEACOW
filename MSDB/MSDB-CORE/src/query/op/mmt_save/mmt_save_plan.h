#pragma once
#ifndef _MSDB_OP_MMT_SAVE_PLAN_H_
#define _MSDB_OP_MMT_SAVE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_save_plan : public opPlan
{
public:
	mmt_save_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_save_pset : public opArrayParamSet
{
public:
	mmt_save_pset(parameters& pSet);
};
}	// msdb

#endif		// _MSDB_OP_MMT_SAVE_PLAN_H_
