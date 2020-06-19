#include <pch.h>
#include <array/memChunk.h>
#include <array/dimensionDesc.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <cmath>
#include <vector>

namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

TEST(wavelet_encode_action, waveletHaar_1D)
{
	////////////////////////////////
	//// Set up dummy data
	//wavelet w("Haar");
	//double data[] = { 1,2,3,4,5,6,7,8 };
	//double expectedEncoding[] = {
	//	2.12132034, 4.94974747, 7.77817459, 10.60660172,
	//	-0.70710678, -0.70710678, -0.70710678, -0.70710678
	//};
	dimension dim = { 8 };
	//position_t csP[] = { 0 };
	//position_t ceP[] = { 8 };

	////////////////////////////////
	//// Set up array
	//pAttributeDesc attrDesc = std::make_shared<attributeDesc>();
	//attrDesc->id_ = 0;
	//attrDesc->name_ = "v1";
	//attrDesc->type_ = eleType::DOUBLE;

	//coor sP(1, csP);
	//coor eP(1, ceP);

	//pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDesc, dim, sP, eP);
	//pChunk sourceChunk = std::make_shared<chunk>(cDesc);
	//sourceChunk->materializeAssign(data, sizeof(data));


	//auto it = sourceChunk->getItemIterator();

	////////////////////////////////
	//// Encoding
	//double* oEncoding = (double*)malloc(sizeof(data));
	//waveletEncode(&w, data, oEncoding, sizeof(data) / sizeof(double), &dims);
	//for (unsigned int i = 0; i < sizeof(data) / sizeof(double); i++)
	//{
	//	EXPECT_EQ(ROUNDING(oEncoding[i], 6), ROUNDING(expectedEncoding[i], 6));
	//}

	////////////////////////////////
	//// Decoding
	//double* oDecoding = (double*)malloc(sizeof(data));
	//waveletDecode(oDecoding, oEncoding, &w, sizeof(data) / sizeof(double), dims, 0);
	//for (size_t i = 0; i < sizeof(data) / sizeof(double); i++)
	//{
	//	std::cout << oDecoding[i] << " <-> " << data[i] << std::endl;
	//	EXPECT_EQ(ROUNDING(oDecoding[i], 6), data[i]);
	//}

	//free(oEncoding);
	//free(oDecoding);
}
}
