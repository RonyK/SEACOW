#pragma once
#ifndef _MSDB_OP_BETWEEN_PLAN_H_
#define _MSDB_OP_BETWEEN_PLAN_H_

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
		virtual pAction getAction() override;
	};

	class between_pset : public opParamSet
	{
	public:
		between_pset();
		between_pset(parameters& pSet);

	public:
		virtual pArrayDesc inferSchema() override;
	};
}

#endif // _MSDB_OP_BETWEEN_PLAN_H_
