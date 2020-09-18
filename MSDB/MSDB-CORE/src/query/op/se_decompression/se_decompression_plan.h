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
	virtual pAction makeAction() override;
};

class se_decompression_array_pset : public opArrayParamSet
{
public:
	se_decompression_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
};

class se_decompression_plan_pset : public opPlanParamSet
{
public:
	se_decompression_plan_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// msdb
#endif	// _MSDB_SE_DECOMPRESSION_PLAN_H_