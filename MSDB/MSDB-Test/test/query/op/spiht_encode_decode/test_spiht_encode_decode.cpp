#include <pch.h>
#include <compression/testCompression.h>
#include <util/timer.h>

namespace msdb
{
namespace caDummy
{

template <typename value_type>
void test_body_spiht(_pFuncGetSourceArray_, eleDefault wtLevel)
{
	timer myTimer;
	bool printFlag = false;

	//////////////////////////////
	myTimer.start(0, "", workType::ARRAY_CONSTRUCTING);
	// Assing new array id for spiht encode array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 4);	// 44445
	myTimer.nextWork(0, workType::COMPUTING);

	//////////////////////////////
	auto arr_wavelet_encode = wavelet_encode(sourceArr, wtLevel);
	myTimer.nextWork(0, workType::COMPUTING);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	myTimer.nextWork(0, workType::LOGGING);
	if (printFlag)
	{
		arr_wavelet_encode->print();
	}

	//////////////////////////////
	auto arr_spiht_encode = spiht_encode(std::vector<pArray>({ arr_wavelet_encode }));
	myTimer.nextWork(0, workType::COMPUTING);
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Encode Arr" << std::endl;
	myTimer.nextWork(0, workType::LOGGING);

	//////////////////////////////
	auto arr_spiht_decode = spiht_decode(std::vector<pArray>({ arr_spiht_encode }));
	myTimer.nextWork(0, workType::COMPUTING);
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_spiht_decode->print();
	}
	myTimer.nextWork(0, workType::LOGGING);

	//////////////////////////////
	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_spiht_decode }), wtLevel);
	myTimer.nextWork(0, workType::COMPUTING);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	if (printFlag)
	{
		arr_wavelet_decode->print();
	}
	myTimer.nextWork(0, workType::LOGGING);

	//////////////////////////////
	compArrary<value_type>(sourceArr[0], arr_wavelet_decode);

	//////////////////////////////
	myTimer.printTime();
	//EXPECT_TRUE(false);
}

namespace data2D_star1024x1024
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_star1024x1024)
{
	test_body_spiht<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}
}
}