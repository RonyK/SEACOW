#pragma once
#include <pch.h>

namespace msdb
{
namespace caDummy
{
pArray mmt_build(std::vector<pArray> sourceArr, eleDefault level);
pArray mmt_save(std::vector<pArray> sourceArr);
pArray mmt_load(std::vector<pArray> sourceArr);
pArray mmt_delta_encode(std::vector<pArray> sourceArr);
pArray mmt_delta_decode(std::vector<pArray> sourceArr);

namespace data2D_sc4x4
{
//void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr = std::vector<pArray>());

void mmt_build_test(pArray afterArray);
void mmt_delta_encode_test(std::shared_ptr<mmt_delta_encode_array> arr);
void mmt_delta_decode_test(std::shared_ptr<mmt_delta_decode_array> arr);
std::shared_ptr<mmt_delta_encode_array> get_mmt_delta_encode_array();
}	// data2D_sc4x4
}	// caDummy
}	// msdb