#pragma once
#ifndef _MSDB_OP_SPIHT_ENCODE_PLAN_H_
#define _MSDB_OP_SPIHT_ENCODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class spiht_encode_plan : public opPlan
{
public:
	spiht_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class spiht_encode_pset : public opParamSet
{
public:
	spiht_encode_pset();
	spiht_encode_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif // _MSDB_OP_SPIHT_ENCODE_PLAN_H_