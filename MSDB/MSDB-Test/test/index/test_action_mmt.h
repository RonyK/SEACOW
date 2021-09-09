#pragma once
#include <pch_test.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
pArray exe_act_ind_mmt_build(std::vector<pArray> sourceArr, eleDefault level);
pArray exe_act_ind_mmt_save(std::vector<pArray> sourceArr);
pArray exe_act_ind_mmt_load(std::vector<pArray> sourceArr);
pArray exe_act_ind_mmt_delta_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_mmt_delta_decode(std::vector<pArray> sourceArr);

namespace data2D_sc4x4
{
//void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr = std::vector<pArray>());

void mmt_build_test(pArray afterArray);
void mmt_delta_encode_test(std::shared_ptr<mmt_delta_encode_array> arr);
void mmt_delta_decode_test(std::shared_ptr<mmt_delta_decode_array> arr);
std::shared_ptr<mmt_delta_encode_array> get_mmt_delta_encode_array();
}		// data2D_sc4x4
}		// caDummy
}		// core
}		// msdb