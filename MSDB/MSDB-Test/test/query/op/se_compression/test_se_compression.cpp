#include <pch.h>

#include <compression/testCompression.h>
#include <compression/testSeCompression.h>
#include <system/storageMgr.h>
#include <io/testIO.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_se_compression_decompression(_pFuncGetSourceArray_, _pFuncGetSourceArrayDesc_, eleDefault wtLevel, eleDefault mmtLevel)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);

	action_execute_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
	action_execute_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = action_execute_se_decompression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);

	return outArr;
}

template <typename value_type>
pArray test_se_compression_decompression_seq(_pFuncGetSourceArray_,
											 _pFuncGetSourceArrayDesc_,
											 eleDefault wtLevel,
											 eleDefault mmtLevel,
											 bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + 2);

	action_execute_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
	action_execute_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = action_execute_se_decompression_seq<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	return outArr;
}

namespace data2D_sc4x4
{
TEST(query_op_se_compression, se_compression_sc4x4)
{
	test_body_se_compression_decompression<value_type>(&getSourceArrayIfEmpty, 
													   &getSourceArrayDesc, 
													   wtLevel, mmtLevel);		// 443
}

TEST(query_op_se_compression, se_comrpession_seq_sc4x4)
{
	bool printFlag = false;
	test_se_compression_decompression_seq<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  wtLevel, mmtLevel,
													  printFlag);				// 443
}
}   // data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, se_compression_star1024x1024)
{
	test_body_se_compression_decompression<value_type>(&getSourceArrayIfEmpty,
													   &getSourceArrayDesc, 
													   wtLevel, mmtLevel);		// 24243
}

TEST(query_op_se_compression, se_comrpession_seq_star1014x1024)
{
	bool printFlag = false;
	test_se_compression_decompression_seq<value_type>(&getSourceArrayIfEmpty,
													  &getSourceArrayDesc,
													  wtLevel, mmtLevel,
													  printFlag);				// 24243
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