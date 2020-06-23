#pragma once
#ifndef _MSDB_OP_ENCODE_PLAN_H_
#define _MSDB_OP_ENCODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_encode_plan : public opPlan
{
public:
	mmt_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_encode_pset : public opParamSet
{
public:
	mmt_encode_pset();
	mmt_encode_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}	// msdb

#endif		// _MSDB_OP_ENCODE_PLAN_H_
