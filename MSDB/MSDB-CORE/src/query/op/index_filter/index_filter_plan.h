#pragma once
#ifndef _MSDB_INDEX_FILTER_PLAN_H_
#define _MSDB_INDEX_FILTER_PLNA_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class index_filter_plan : public opPlan
{
public:
	index_filter_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class index_filter_array_pset : public opArrayParamSet
{
public:
	index_filter_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}		// msdb
#endif	// _MSDB_INDEX_FILTER_PLAN_H_