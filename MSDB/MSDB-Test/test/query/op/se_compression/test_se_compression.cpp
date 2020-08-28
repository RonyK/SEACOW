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
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	arr_wavelet_encode->print();

	auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Se Compression Arr" << std::endl;
	arr_se_compression->print();

	auto arr_se_decompression = se_decompression(std::vector<pArray>({ arr_delta_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Se Decompression Arr" << std::endl;
	arr_se_decompression->print();

	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_se_decompression }));
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	arr_wavelet_decode->print();

	auto arr_delta_decode = mmt_delta_decode(std::vector<pArray>({ arr_wavelet_decode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Decode Arr" << std::endl;
	arr_delta_decode->print();

	load_test(arr_delta_decode);
	//EXPECT_TRUE(false);
}
}   // data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, se_compression_star1024x1024)
{
	bool printFlag = false;

	// Assing new array id for se compressed array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);     // 24243

	auto arr_mmt_build = mmt_build(sourceArr);
	std::cout << "##############################" << std::endl;
	std::cout << "Source Arr" << std::endl;
	if (printFlag)
	{
		arr_mmt_build->print();
	}

	auto arr_delta_encode = mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Arr" << std::endl;
	if (printFlag)
	{
		arr_delta_encode->print();
	}

	auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_encode->print();
	}

	auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Se Compression Arr" << std::endl;
	if (printFlag)
	{
		arr_se_compression->print();
	}

	auto arr_se_decompression = se_decompression(std::vector<pArray>({ arr_delta_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Se Decompression Arr" << std::endl;
	if (printFlag)
	{
		arr_se_decompression->print();
	}

	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_se_decompression }));
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_decode->print();
	}

	auto arr_delta_decode = mmt_delta_decode(std::vector<pArray>({ arr_wavelet_decode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_delta_decode->print();
	}

	//compArrary<value_type>(arr_wavelet_encode, arr_se_decompression);
	//compArrary<value_type>(arr_delta_encode, arr_wavelet_decode);
	compArrary<value_type>(arr_mmt_build, arr_delta_decode);
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb