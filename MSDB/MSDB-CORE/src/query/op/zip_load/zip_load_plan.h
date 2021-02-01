#pragma once
#ifndef _MSDB_OP_ZIP_DECODE_PLAN_H_
#define _MSDB_OP_ZIP_DECODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class zip_load_plan : public opPlan
{
public:
	zip_load_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class zip_load_array_pset : public opArrayParamSet
{
public:
	zip_load_array_pset(parameters& pSet);
};
}

#endif // _MSDB_OP_ZIP_DECODE_PLAN_H_