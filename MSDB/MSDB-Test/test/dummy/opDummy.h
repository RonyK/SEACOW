#pragma once
#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

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
}
}