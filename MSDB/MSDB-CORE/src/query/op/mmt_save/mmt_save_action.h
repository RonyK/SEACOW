#pragma once
#ifndef _MSDB_OP_mmt_save_ACTION_H_
#define _MSDB_OP_mmt_save_ACTION_H_

#include <query/opAction.h>

namespace msdb
{
class mmt_save_action : public opAction
{
public:
	mmt_save_action();
	virtual ~mmt_save_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

	virtual const char* name() override;

};
}	// msdb

#endif		// _MSDB_OP_mmt_save_ACTION_H_