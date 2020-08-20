#pragma once
#include <pch.h>


namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

namespace caDummy
{
pArray wavelet_encode_execute(std::vector<pArray> sourceArr, eleDefault level);
pArray wavelet_decode_execute(std::vector<pArray> sourceArr, eleDefault level);

namespace data2D_sc4x4
{
pArray wavelet_encode(std::vector<pArray> sourceArr);
void wavelet_encode_check(pArray arr);

pArray wavelet_decode(std::vector<pArray> sourceArr);
void wavelet_decode_check(pArray arr);

pArray se_compression(std::vector<pArray> sourceArr);
pArray se_decompression(std::vector<pArray> sourceArr);
void se_decompression_check(pArray arr);
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
pArray wavelet_encode(std::vector<pArray> sourceArr);
pArray wavelet_decode(std::vector<pArray> sourceArr);

pArray spiht_encode(std::vector<pArray> sourceArr);
pArray spiht_decode(std::vector<pArray> sourceArr);

pArray se_compression(std::vector<pArray> sourceArr);
pArray se_decompression(std::vector<pArray> sourceArr);
}	// data2D_star1024x1024

namespace data2D_tempTest
{
pArray wavelet_encode(std::vector<pArray> sourceArr);
pArray wavelet_decode(std::vector<pArray> sourceArr);

pArray spiht_encode(std::vector<pArray> sourceArr);
pArray spiht_decode(std::vector<pArray> sourceArr);
}	// data2D_tempTest
}	// caDummy
}	// msdb
