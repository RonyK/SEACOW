#include <pch.h>
#include <index/testMMT.h>
#include <compression/testCompression.h>
#include <system/storageMgr.h>
#include <io/testIO.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_se_compression, se_compression_sc4x4)
{
    auto arr_mmt_build = mmt_build();
    auto arr_delta_encode = mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
    auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }));
    auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
}
}   // data2D_sc4x4
}	// caDummy
}	// msdb