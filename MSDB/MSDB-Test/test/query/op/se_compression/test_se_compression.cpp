#include <pch.h>
#include <index/testMMT.h>
#include <compression/testCompression.h>
#include <system/storageMgr.h>
#include <io/testIO.h>
#include <util/timer.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
void test_body_se_compression(_pFuncGetSourceArray_, eleDefault wtLevel, eleDefault mmtLevel)
{
	timer myTimer;
	bool printFlag = false;
	
	myTimer.start(0);
	// Assing new array id for se compressed array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);
	myTimer.check(0, timer::workType::ARRAY_CONSTRUCTING, true);
	//------------------------------

	auto arr_mmt_build = mmt_build(sourceArr, mmtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Source Arr" << std::endl;
	if (printFlag)
	{
		arr_mmt_build->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_delta_encode = mmt_delta_encode(std::vector<pArray>({ arr_mmt_build }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Arr" << std::endl;
	if (printFlag)
	{
		arr_delta_encode->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }), wtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_encode->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_se_compression = se_compression(std::vector<pArray>({ arr_wavelet_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Se Compression Arr" << std::endl;
	if (printFlag)
	{
		arr_se_compression->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_se_decompression = se_decompression(std::vector<pArray>({ arr_delta_encode }), wtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Se Decompression Arr" << std::endl;
	if (printFlag)
	{
		arr_se_decompression->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_se_decompression }), wtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_decode->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	auto arr_delta_decode = mmt_delta_decode(std::vector<pArray>({ arr_wavelet_decode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_delta_decode->print();
	}
	myTimer.check(0, timer::workType::COMPUTING, true);
	//------------------------------

	//compArrary<value_type>(arr_wavelet_encode, arr_se_decompression);
	//compArrary<value_type>(arr_delta_encode, arr_wavelet_decode);
	compArrary<value_type>(arr_mmt_build, arr_delta_decode);

	myTimer.printTime();
	//EXPECT_TRUE(false);
}

namespace data2D_sc4x4
{
TEST(query_op_se_compression, se_compression_sc4x4)
{
	test_body_se_compression<value_type>(&getSourceArrayIfEmpty, wtLevel, mmtLevel);		// 443
}
}   // data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, se_compression_star1024x1024)
{
	test_body_se_compression<value_type>(&getSourceArrayIfEmpty, wtLevel, mmtLevel);		// 24243
}

TEST(query_op_se_compression, delta_spiht_star1024x1024)
{
	bool printFlag = false;

	// Assing new array id for se compressed array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);     // 24243

	auto arr_mmt_build = mmt_build(sourceArr, mmtLevel);
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

	auto arr_wavelet_encode = wavelet_encode(std::vector<pArray>({ arr_delta_encode }), wtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_encode->print();
	}

	auto arr_spiht_encode = spiht_encode(std::vector<pArray>({ arr_wavelet_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Encode Arr" << std::endl;
	if (printFlag)
	{
		arr_spiht_encode->print();
	}

	auto arr_spiht_decode = spiht_decode(std::vector<pArray>({ arr_spiht_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_spiht_decode->print();
	}

	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_spiht_decode }), wtLevel);
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

	EXPECT_TRUE(false);
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb