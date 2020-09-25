#pragma once
#ifndef _MSDB_OP_MMT_LOAD_ACTION_H_
#define _MSDB_OP_MMT_LOAD_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>

namespace msdb
{
class mmt_load_action : public opAction
{
public:
	mmt_load_action();
	virtual ~mmt_load_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}	// msdb

#endif	// _MSDB_OP_MMT_LOAD_ACTION_H_