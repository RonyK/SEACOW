#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_PLAN_H_
#define _MSDB_OP_SPIHT_DECODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class spiht_decode_plan : public opPlan
{
public:
	spiht_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class spiht_decode_pset : public opArrayParamSet
{
public:
	spiht_decode_pset(parameters& pSet);
};
}

#endif // _MSDB_OP_SPIHT_DECODE_PLAN_H_