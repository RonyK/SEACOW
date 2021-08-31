#pragma once
#ifndef _MSDB_OP_SAVE_ACTION_H_
#define _MSDB_OP_SAVE_ACTION_H_

#include <stdafx.h>
#include <array/arrayMgr.h>
#include <query/opAction.h>
#include <index/mmt.h>

namespace msdb
{
namespace core
{
class save_action : public opAction
{
public:
	save_action();
	~save_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SAVE_ACTION_H_