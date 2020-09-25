 #pragma once
#ifndef _MSDB_OP_LOAD_ACTION_H_
#define _MSDB_OP_LOAD_ACTION_H_

#include <stdafx.h>
#include <array/arrayMgr.h>
#include <query/opAction.h>
#include <index/mmt.h>

namespace msdb
{
class load_action : public opAction
{
public:
	load_action();
	~load_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}	// msdb
#endif		// _MSDB_OP_LOAD_ACTION_H_