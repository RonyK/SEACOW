#pragma once
#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_save/mmt_save_action.h>

#include <op/mmt_load/mmt_load_plan.h>
#include <op/mmt_load/mmt_load_action.h>

#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>

#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

namespace msdb
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc sourceArrDesc, eleDefault level,
                      std::shared_ptr<wavelet_encode_plan>& plan, 
                      std::shared_ptr<wavelet_encode_action>& action,
                      pQuery& qry);

void getWaveletDecode(pArrayDesc sourceArrDesc, eleDefault level,
                      std::shared_ptr<wavelet_decode_plan>& plan,
                      std::shared_ptr<wavelet_decode_action>& action,
                      pQuery& qry);

void getMmtBuild(pArrayDesc sourceArrDesc, eleDefault level,
                 std::shared_ptr<mmt_build_plan>& plan,
                 std::shared_ptr<mmt_build_action>& action,
                 pQuery& qry);

void getMmtSave(pArrayDesc sourceArrDesc,
                std::shared_ptr<mmt_save_plan>& plan,
                std::shared_ptr<mmt_save_action>& action,
                pQuery& qry);

void getMmtLoad(pArrayDesc sourceArrDesc,
                std::shared_ptr<mmt_load_plan>& plan,
                std::shared_ptr<mmt_load_action>& action,
                pQuery& qry);

void getMmtDeltaEncode(pArrayDesc sourceArrDesc,
                       std::shared_ptr<mmt_delta_encode_plan>& plan,
                       std::shared_ptr<mmt_delta_encode_action>& action,
                       pQuery& qry);

void getMmtDeltaDecode(pArrayDesc sourceArrDesc,
                       std::shared_ptr<mmt_delta_decode_plan>& plan,
                       std::shared_ptr<mmt_delta_decode_action>& action,
                       pQuery& qry);
}
}