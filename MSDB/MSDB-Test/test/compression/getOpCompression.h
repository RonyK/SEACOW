#pragma once

#ifndef _MSDB_GET_COMPRESSION_OP_H_
#define _MSDB_GET_COMPRESSION_OP_H_

#include <pch_test.h>

#include <dummy/opArrayDummy.h>

#include <op/zip_save/zip_save_plan.h>
#include <op/zip_save/zip_save_action.h>
#include <op/zip_load/zip_load_plan.h>
#include <op/zip_load/zip_load_action.h>

#include <op/lzw_encode/lzw_encode_plan.h>
#include <op/lzw_encode/lzw_encode_action.h>
#include <op/lzw_decode/lzw_decode_plan.h>
#include <op/lzw_decode/lzw_decode_action.h>

namespace msdb
{
namespace core
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

void getOpLzwEncode(pArrayDesc sourceArrDesc,
					std::shared_ptr<lzw_encode_plan>& plan,
					std::shared_ptr<lzw_encode_action>& action,
					pQuery& qry);

void getOpLzwDecode(pArrayDesc sourceArrDesc,
					std::shared_ptr<lzw_decode_plan>& plan,
					std::shared_ptr<lzw_decode_action>& action,
					pQuery& qry);
}		// caDummy
}		// core
}		// msdb
#endif	// _MSDB_GET_COMPRESSION_OP_H_
