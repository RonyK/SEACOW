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
    // Assing new array id for se compressed array
    std::vector<pArray> sourceArr;
    getSourceArrayIfEmpty(sourceArr);
    sourceArr[0]->setId(sourceArr[0]->getId() + 2);     // 443

    auto arr_mmt_build = mmt_build(sourceArr);
    std::cout << "##############################" << std::endl;
    std::cout << "Source Arr" << std::endl;
    arr_mmt_build->print();

    auto arr_delta_encode = mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
    std::cout << "##############################" << std::endl;
    std::cout << "Delta Arr" << std::endl;
    arr_delta_encode->print();

    auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }));
    auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
}
}   // data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, se_compression_star1024x1024)
{
    // Assing new array id for se compressed array
    std::vector<pArray> sourceArr;
    getSourceArrayIfEmpty(sourceArr);
    sourceArr[0]->setId(sourceArr[0]->getId() + 2);     // 443

    auto arr_mmt_build = mmt_build(sourceArr);
    std::cout << "##############################" << std::endl;
    std::cout << "Source Arr" << std::endl;
    arr_mmt_build->print();

    auto arr_delta_encode = mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
    std::cout << "##############################" << std::endl;
    std::cout << "Delta Arr" << std::endl;
    arr_delta_encode->print();

    auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }));
    auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb