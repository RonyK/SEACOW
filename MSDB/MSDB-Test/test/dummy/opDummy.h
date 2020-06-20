#pragma once
#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>

namespace msdb
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc arrDesc, eleDefault level,
                      std::shared_ptr<wavelet_encode_plan> plan, 
                      std::shared_ptr<wavelet_encode_action> action,
                      pQuery q);
}
}