#pragma once
#ifndef _MSDB_OP_BETWEEN_PLAN_H_
#define _MSDB_OP_BETWEEN_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
	class between_plan : public opPlan
	{
	public:
		between_plan();

	public:
		virtual const char* name() override;
		virtual pAction makeAction() override;
	};

	class between_array_pset : public opArrayParamSet
	{
	public:
		between_array_pset(parameters& pSet);

	public:
		virtual pArrayDesc inferSchema() override;
	};
}

#endif // _MSDB_OP_BETWEEN_PLAN_H_
