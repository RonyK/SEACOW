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
	double data[] = { 1,2,3,4,5,6,7,8 };
	double expectedEncoding[] = {
		2.12132034, 4.94974747, 7.77817459, 10.60660172,
		-0.70710678, -0.70710678, -0.70710678, -0.70710678
	};
	dimension dim = { 8 };
	coor sP = { 0 };	// chunk start point
	coor eP = { 8 };	// chunk end point

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
	weAction->waveletName_ = "Haar";
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

TEST(wavelet_encode_action, waveletHaarSimple_2D)
{
	//////////////////////////////
	// Set up dummy data
	double data[][4] = { {9,7,6,2}, {5,3,4,4}, {8,2,4,0}, {6,0,2,2} };
	double expectedOutput[] = {
		6, 4, 4, 2,  1, 1, 3, 1,  2, 0, 1, 0,  0, 1, 0, 1
	};
	dimension dim = { sizeof(data) / sizeof(double) / 4, 4 };
	coor sP = { 0, 0 };
	coor eP = { 4, 4 };

	//////////////////////////////
	// Set up array
	dimensionDescs dimDescs;
	dimDescs.push_back(std::make_shared<dimensionDesc>(0, "X", 0, 4, 4));
	dimDescs.push_back(std::make_shared<dimensionDesc>(0, "Y", 0, 4, 4));
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
	for (size_t i = 0; i < sizeof(data) / sizeof(double); )
	{
		auto it = weArray->getChunk(cId)->getItemIterator();
		if (it.getCapacity() == 0)
		{
			throw std::exception();
		}
		for (size_t j = 0; j < it.getCapacity(); ++j, ++i)
		{
			std::cout << (*it).getDouble() << " <> " << expectedOutput[i] << std::endl;
			EXPECT_EQ(ROUNDING((*it).getDouble(), 6), ROUNDING(expectedOutput[i], 6));
			++it;
		}
		++cId;
	}
}

namespace caDummy
{
namespace data2D_sc4x4
{
TEST(wavelet_encode_action, waveletHaarSimple_sc4x4)
{
	std::vector<pArray> sourceArr = getSourceArray();
	
	//////////////////////////////
	// source array value test
	//auto aDesc = sourceArr[0]->getDesc();
	//for(int y = 0; y < aDesc->dimDescs_[0]->getChunkNum(); y++)
	//{
	//	for(int x = 0; x < aDesc->dimDescs_[1]->getChunkNum(); x++)
	//	{
	//		size_t cId = x + y * aDesc->dimDescs_[1]->getChunkNum();
	//		std::cout << "chunk " << cId << " (" << x << ", " << y << ")" << std::endl;
	//		auto it = sourceArr[0]->getChunk(cId)->getItemIterator();
	//		
	//		for(int iy = 0; iy < aDesc->dimDescs_[0]->chunkSize_; iy++)
	//		{
	//			for (int ix = 0; ix < aDesc->dimDescs_[1]->chunkSize_; ix++)
	//			{
	//				std::cout << static_cast<int>((*it).getChar()) << ", ";
	//				++it;
	//			}
	//			std::cout << std::endl;
	//		}
	//	}
	//}
	//std::cout << "=====" << std::endl;

	eleDefault level = 0;
	std::shared_ptr<wavelet_encode_plan> wePlan;
	std::shared_ptr<wavelet_encode_action> weAction;
	pQuery weQuery;
	getWaveletEncode(sourceArr[0]->getDesc(), level, wePlan, weAction, weQuery);

	auto weArray = weAction->execute(sourceArr, weQuery);

	//////////////////////////////
	// Check Encoded Result
	value_type expected[dataLength] = { 0 };
	getWTDummy(expected, dataLength);
	size_t cId = 0;
	for (size_t i = 0; i < sizeof(dataLength) / sizeof(double); )
	{
		auto it = weArray->getChunk(cId)->getItemIterator();
		if (it.getCapacity() == 0)
		{
			throw std::exception();
		}
		for (size_t j = 0; j < it.getCapacity(); ++j, ++i)
		{
			std::cout << static_cast<int>((*it).getChar()) << " <> " << static_cast<int>(expected[i]) << std::endl;
			EXPECT_EQ(ROUNDING((*it).getChar(), 6), ROUNDING(expected[i], 6));
			++it;
		}
		++cId;
	}
}
}	// data2D_sc4x4
}	// caDummy
}	// msdb
