#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_PLAN_H_
#define _MSDB_OP_WAVELET_ENCODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class wavelet_encode_plan : public opPlan
{
public:
	wavelet_encode_plan();

public:
	virtual const char* name() override;
	//virtual void initParamSets() override;
	virtual pAction getAction() override;

};

class wavelet_encode_pset : public opArrayParamSet
{
public:
	wavelet_encode_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_PLAN_H_