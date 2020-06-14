#pragma once
#ifndef _MSDB_OPPLAN_H_
#define _MSDB_OPPLAN_H_

#include <array/arrayDesc.h>
#include <query/opParamSet.h>
#include <vector>
#include <memory>

namespace msdb
{
class opPlan
{
public:
	opPlan();

public:
	virtual const char* name() = 0;
	virtual void initParamSets() = 0;
	virtual arrayDesc inferSchema(paramSet oppSet);
	void addParamSet(paramSet pSet);

private:
	std::vector<paramSet> paramSets_;
};
}

#endif		// _MSDB_OPPLAN_H_
