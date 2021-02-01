#pragma once

#ifndef _MSDB_GET_COMPRESSION_OP_H_
#define _MSDB_GET_COMPRESSION_OP_H_

#include <pch.h>

#include <dummy/opArrayDummy.h>

#include <op/zip_save/zip_save_plan.h>
#include <op/zip_save/zip_save_action.h>
#include <op/zip_load/zip_load_plan.h>
#include <op/zip_load/zip_load_action.h>

namespace msdb
{
namespace caDummy
{
void getOpZipSave(pArrayDesc sourceArrDesc,
				  std::shared_ptr<zip_save_plan>& plan,
				  std::shared_ptr<zip_save_action>& action,
				  pQuery& qry);

void getOpZipLoad(pArrayDesc sourceArrDesc,
				  std::shared_ptr<zip_load_plan>& plan,
				  std::shared_ptr<zip_load_action>& action,
				  pQuery& qry);
}		// caDummy
}		// msdb
#endif	// _MSDB_GET_COMPRESSION_OP_H_
