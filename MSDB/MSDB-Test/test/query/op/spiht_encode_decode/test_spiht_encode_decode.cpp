#include <pch.h>
#include <compression/testCompression.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_star1024x1024)
{
	bool printFlag = false;

	// Assing new array id for spiht encode array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 4);	// 44445

	//auto weSource = getSourceArray<wavelet_encode_array>();
	//weSource[0]->setOrigianlChunkDims(chunkDims);
	//weSource[0]->setMaxLevel(maxLevel);
	//std::vector<pArray> weSourceList = { std::static_pointer_cast<arrayBase>(weSource[0]) };

	//auto arr_wavelet_decode2 = wavelet_decode(weSourceList);

	//arr_wavelet_decode2->print();
	//EXPECT_TRUE(false);

	// sourceArr[0]->print();
	auto arr_wavelet_encode = wavelet_encode(sourceArr);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Encode Arr" << std::endl;
	// arr_wavelet_encode->print();
	
	auto arr_spiht_encode = spiht_encode(std::vector<pArray>({ arr_wavelet_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Encode Arr" << std::endl;

	auto arr_spiht_decode = spiht_decode(std::vector<pArray>({ arr_spiht_encode }));
	std::cout << "##############################" << std::endl;
	std::cout << "SPIHT Decode Arr" << std::endl;
	// arr_spiht_decode->print();

	auto arr_wavelet_decode = wavelet_decode(std::vector<pArray>({ arr_spiht_decode }));
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	// arr_wavelet_decode->print();

	compArrary<value_type>(sourceArr[0], arr_wavelet_decode);
	//EXPECT_TRUE(false);
}
}
}
}