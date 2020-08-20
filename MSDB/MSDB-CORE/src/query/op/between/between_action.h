#pragma once
#ifndef _MSDB_OP_BETWEEN_ACTION_H_
#define _MSDB_OP_BETWEEN_ACTION_H_

#include <query/opAction.h>

namespace msdb
{
	class between_action : public opAction
	{
	public:
		between_action();
		virtual ~between_action();
		virtual const char* name() override;

	public:
		pArray execute(std::vector<pArray>& inputArrays, pQuery q);
	};
}

#endif // _MSDB_OP_BETWEEN_ACTION_H_
