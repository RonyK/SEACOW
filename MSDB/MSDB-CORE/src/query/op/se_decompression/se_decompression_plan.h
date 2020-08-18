#pragma once
#ifndef _MSDB_SE_DECOMPRESSION_PLAN_H_
#define _MSDB_SE_DECOMPRESSION_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class se_decompression_plan : public opPlan
{
public:
	se_decompression_plan();
	~se_decompression_plan();

public:
	virtual const char* name() override;
	virtual pAction getAction() override;
};

class se_decompression_pset : public opParamSet
{
public:
	se_decompression_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}		// msdb
#endif	// _MSDB_SE_DECOMPRESSION_PLAN_H_