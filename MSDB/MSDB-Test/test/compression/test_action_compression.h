#pragma once
#include <pch.h>
namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

namespace caDummy
{
pArray exe_act_ind_wavelet_encode(std::vector<pArray> sourceArr, eleDefault level);
pArray exe_act_ind_wavelet_decode(std::vector<pArray> sourceArr, eleDefault level);

pArray exe_act_ind_spiht_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_spiht_decode(std::vector<pArray> sourceArr, eleDefault level);

pArray exe_act_ind_se_compression(std::vector<pArray> sourceArr);
pArray exe_act_ind_se_decompression(std::vector<pArray> sourceArr, eleDefault level);

pArray exe_act_ind_compass_encode(std::vector<pArray> sourceArr, eleDefault numBins);
pArray exe_act_ind_compass_decode(std::vector<pArray> sourceArr, eleDefault numBins);

pArray exe_act_ind_compass_index_build(std::vector<pArray> sourceArr, eleDefault numBins);

pArray exe_act_ind_zip_save(std::vector<pArray> sourceArr);
pArray exe_act_ind_zip_load(std::vector<pArray> sourceArr);
}	// caDummy
}	// msdb
