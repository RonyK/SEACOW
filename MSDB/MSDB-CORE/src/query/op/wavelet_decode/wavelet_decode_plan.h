#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_PLAN_H_
#define _MSDB_OP_WAVELET_DECODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class wavelet_decode_plan : public opPlan
{
public:
	wavelet_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class wavelet_decode_pset : public opArrayParamSet
{
public:
	wavelet_decode_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif		// _MSDB_OP_WAVELET_DECODE_PLAN_H_