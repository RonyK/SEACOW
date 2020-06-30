#pragma once
#include <pch.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr = std::vector<pArray>());

pArray mmt_build(std::vector<pArray> sourceArr = std::vector<pArray>());
pArray mmt_save(std::vector<pArray> sourceArr = std::vector<pArray>());
pArray mmt_load(std::vector<pArray> sourceArr = std::vector<pArray>());
pArray mmt_delta_encode(std::vector<pArray> sourceArr = std::vector<pArray>());

}	// data2D_sc4x4
}	// caDummy
}	// msdb