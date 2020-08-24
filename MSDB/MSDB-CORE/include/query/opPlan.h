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
	virtual pArrayDesc inferSchema();
	virtual pBitmap inferBitmap();
	virtual pBitmap inferBottomUpBitmap();
	virtual pBitmap inferTopDownBitmap();
	virtual pAction getAction() = 0;
	parameters getParam();

protected:
	virtual void setParentPlan(pPlan parentPlan);

private:
	pParamSet inParamSet_;
	pBitmap outArrBitmap_;
	pPlan parentPlan_;
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
class opPlanParamSet : public opParamSet
{
public:
	opPlanParamSet(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmap inferBottomUpBitmap() override;
	virtual pBitmap inferTopDownBitmap(pBitmap fromParent) override;
};
}

#endif		// _MSDB_OPPLAN_H_
