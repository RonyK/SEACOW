#pragma once
#include <pch.h>


namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

namespace caDummy
{
pArray wavelet_encode(std::vector<pArray> sourceArr, eleDefault level);
pArray wavelet_decode(std::vector<pArray> sourceArr, eleDefault level);

pArray spiht_encode(std::vector<pArray> sourceArr);
pArray spiht_decode(std::vector<pArray> sourceArr);

pArray se_compression(std::vector<pArray> sourceArr);
pArray se_decompression(std::vector<pArray> sourceArr, eleDefault level);
}	// caDummy
}	// msdb
