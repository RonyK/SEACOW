#pragma once
#include <pch.h>


namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

namespace caDummy
{
namespace data2D_sc4x4
{
pArray wavelet_encode(std::vector<pArray> sourceArr);
void wavelet_encode_check(pArray arr);

pArray wavelet_decode(std::vector<pArray> sourceArr);
void wavelet_decode_check(pArray arr);

pArray se_compression(std::vector<pArray> sourceArr);
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
pArray wavelet_encode(std::vector<pArray> sourceArr);

pArray wavelet_decode(std::vector<pArray> sourceArr);

pArray se_compression(std::vector<pArray> sourceArr);
}	// data2D_star1024x1024
}	// caDummy
}	// msdb
