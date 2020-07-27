#pragma once
#include <pch.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
//void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr = std::vector<pArray>());

pArray mmt_build(std::vector<pArray> sourceArr = std::vector<pArray>());
void mmt_build_test(pArray afterArray);

pArray mmt_save(std::vector<pArray> sourceArr = std::vector<pArray>());
pArray mmt_load(std::vector<pArray> sourceArr = std::vector<pArray>());

pArray mmt_delta_encode(std::vector<pArray> sourceArr = std::vector<pArray>());
void mmt_delta_encode_test(std::shared_ptr<mmt_delta_encode_array> arr);
pArray mmt_delta_decode(std::vector<std::shared_ptr<mmt_delta_encode_array>>& sourceArr);
std::shared_ptr<mmt_delta_encode_array> get_mmt_delta_encode_array();
void mmt_delta_decode_test(std::shared_ptr<mmt_delta_decode_array> arr);

}	// data2D_sc4x4

namespace data2D_sc8x8
{
//void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr = std::vector<pArray>());
pArray mmt_build(std::vector<pArray> sourceArr = std::vector<pArray>());
pArray mmt_save(std::vector<pArray> sourceArr = std::vector<pArray>());
}	// data2D_sc8x8
}	// caDummy
}	// msdb