#pragma once
#ifndef _MSDB_OPPLAN_H_
#define _MSDB_OPPLAN_H_

#include <array/arrayDesc.h>
#include <query/opParamSet.h>
#include <query/opAction.h>
#include <vector>
#include <memory>

namespace msdb
{
class opPlan;
using pPlan = std::shared_ptr<opPlan>;

class opPlan : public std::enable_shared_from_this<opPlan>
{
public:
	opPlan();

public:
	virtual const char* name() = 0;
	void setParamSet(pParamSet paramSet);
	//virtual void initParamSets() = 0;
	virtual pArrayDesc inferSchema();
	virtual pBitmap inferBitmap();
	//void addParamSet(pParamSet pSet);
	virtual pAction getAction() = 0;
	parameters getParam();

private:
	pParamSet myParamSet_;
	//std::vector<pParamSet> paramSets_;
};

class opParamPlan : public opParam
{
public:
	using paramType = opPlan;

public:
	opParamPlan(pPlan plan);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pPlan plan_;
};

class opParamPlanPlaceholder : public opParamPlaceholder, public opParamPlan
{
public:
	opParamPlanPlaceholder();

public:
	virtual opParamType type();
};
}

#endif		// _MSDB_OPPLAN_H_
