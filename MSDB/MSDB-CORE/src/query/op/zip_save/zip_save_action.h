#pragma once
#ifndef _MSDB_OP_ZIP_ENCODE_ACTION_H_
#define _MSDB_OP_ZIP_ENCODE_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <util/math.h>

namespace msdb
{
class zip_save_action : public opAction
{
public:
	zip_save_action();
	virtual ~zip_save_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}	// msdb
#endif	// _MSDB_OP_ZIP_ENCODE_ACTION_H_