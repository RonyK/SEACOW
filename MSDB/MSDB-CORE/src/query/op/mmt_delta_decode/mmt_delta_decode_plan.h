#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_
#define _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_delta_decode_plan : public opPlan
{
public:
	mmt_delta_decode_plan();
	~mmt_delta_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_delta_decode_pset : public opParamSet
{
public:
	mmt_delta_decode_pset();
	mmt_delta_decode_pset(parameters& pSet);
	~mmt_delta_decode_pset();

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif		// _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_