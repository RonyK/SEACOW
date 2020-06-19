#pragma once
#ifndef _MSDB_OPPLAN_H_
#define _MSDB_OPPLAN_H_

#include <array/arrayDesc.h>
#include <query/opParamSet.h>
#include <vector>
#include <memory>

namespace msdb
{
class opPlan : public std::enable_shared_from_this<opPlan>
{
public:
	opPlan();

public:
	virtual const char* name() = 0;
	virtual void initParamSets() = 0;
	virtual pArrayDesc inferSchema(pParamSet oppSet);
	void addParamSet(pParamSet pSet);

private:
	std::vector<pParamSet> paramSets_;
};
}

#endif		// _MSDB_OPPLAN_H_
