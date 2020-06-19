#include <pch.h>
#include <array/memChunk.h>
#include <array/dimensionDesc.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <cmath>
#include <vector>

namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

TEST(wavelet_encode_action, waveletHaar_1D)
{
	//////////////////////////////
	// Set up dummy data
	wavelet w("Haar");
	double data[] = { 1,2,3,4,5,6,7,8 };
	double expectedEncoding[] = {
		2.12132034, 4.94974747, 7.77817459, 10.60660172,
		-0.70710678, -0.70710678, -0.70710678, -0.70710678
	};
	dimension dim = { 8 };
	coor sP = { 0 };	// array start point
	coor eP = { 8 };	// array end point

	//////////////////////////////
	// Set up array
	dimensionDescs dimDescs;
	dimDescs.push_back(std::make_shared<dimensionDesc>(0, "X", 0, 8, 8));
	attributeDescs attrDescs;
	attrDescs.push_back(std::make_shared<attributeDesc>(0, "A1", eleType::DOUBLE));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "wavelet_test_array", dimDescs, attrDescs);
	pArray sourceArr = std::make_shared<arrayBase>(arrDesc);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs[0], dim, sP, eP);
	pChunk sourceChunk = std::make_shared<chunk>(cDesc);
	sourceChunk->materializeCopy(data, sizeof(data));
	sourceArr->insertChunk(sourceChunk);

	//////////////////////////////
	// Encoding
	pQuery q = std::make_shared<query>();
	auto wePlan = std::make_shared<wavelet_encode_plan>();
	auto weAction = std::make_shared<wavelet_encode_action>();
	eleDefault level = 0;
	auto sEle = std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE);
	parameters params = {
		std::make_shared<opParamArray>(arrDesc), 
		std::make_shared<opParamConst>(sEle)
	};

	auto pSet = std::make_shared<wavelet_encode_pset>(params);
	wePlan->setParamSet(pSet);

	weAction->setArrayDesc(wePlan->inferSchema());
	weAction->setParams(params);
	auto weArray = weAction->execute(arrs, q);

	//////////////////////////////
	// Check Encoded Result
	size_t cId = 0;
	for(size_t i = 0; i < sizeof(data) / sizeof(double); )
	{
		auto it = weArray->getChunk(cId)->getItemIterator();
		if (it.getCapacity() == 0)
		{
			throw std::exception();
		}
		for(size_t j = 0; j < it.getCapacity(); ++j, ++i)
		{
			std::cout << (*it).getDouble() << " <> " << expectedEncoding[i] << std::endl;
			EXPECT_EQ(ROUNDING((*it).getDouble(), 6), ROUNDING(expectedEncoding[i], 6));
			++it;
		}
		++cId;
	}

	////////////////////////////////
	//// Decoding
	//double* oDecoding = (double*)malloc(sizeof(data));
	//waveletDecode(oDecoding, oEncoding, &w, sizeof(data) / sizeof(double), dims, 0);

	////////////////////////////////
	//// Check Decoded Result
	//for (size_t i = 0; i < sizeof(data) / sizeof(double); i++)
	//{
	//	std::cout << oDecoding[i] << " <-> " << data[i] << std::endl;
	//	EXPECT_EQ(ROUNDING(oDecoding[i], 6), data[i]);
	//}
}
}
