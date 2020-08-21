#pragma once
#ifndef _MSDB_OP_MMT_DELTA_ENCODE_PLAN_H_
#define _MSDB_OP_MMT_DELTA_ENCODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class mmt_delta_encode_plan : public opPlan
{
public:
	mmt_delta_encode_plan();
	~mmt_delta_encode_plan();

public:
	// opPlan을(를) 통해 상속됨
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class mmt_delta_encode_pset : public opArrayParamSet
{
public:
	mmt_delta_encode_pset(parameters& pSet);
};
}	// msdb
#endif	// _MSDB_OP_MMT_DELTA_ENCODE_PLAN_H_