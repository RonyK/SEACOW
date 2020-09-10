#include <pch.h>
#include <array/memBlockArray.h>
#include <array/memChunk.h>
#include <array/memChunkBuffer.h>
#include <array/dimensionDesc.h>
#include <compression/testCompression.h>
#include <cmath>
#include <vector>

namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

TEST(query_op_wavelet_encode, waveletHaar_1D)
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
	pDimensionDescs dimDescs = std::make_shared<dimensionDescs>();
	dimDescs->push_back(std::make_shared<dimensionDesc>(0, "X", 0, 8, 8, 8));
	pAttributeDescs attrDescs = std::make_shared<attributeDescs>();
	attrDescs->push_back(std::make_shared<attributeDesc>(0, "A1", eleType::DOUBLE));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "wavelet_test_array", dimDescs, attrDescs);
	pArray sourceArr = std::make_shared<memBlockArray>(arrDesc);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs->at(0), dim, dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->bufferCopy(data, sizeof(data));
	sourceArr->insertChunk(0, sourceChunk);

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

	auto pSet = std::make_shared<wavelet_encode_array_pset>(params);
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
		if (it->getCapacity() == 0)
		{
			throw std::exception();
		}
		for(size_t j = 0; j < it->getCapacity(); ++j, ++i)
		{
			std::cout << (**it).getDouble() << " <> " << expectedEncoding[i] << std::endl;
			EXPECT_EQ(ROUNDING((**it).getDouble(), 6), ROUNDING(expectedEncoding[i], 6));
			++(*it);
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

TEST(query_op_wavelet_encode, waveletHaarSimple_2D)
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
	pDimensionDescs dimDescs;
	dimDescs->push_back(std::make_shared<dimensionDesc>(0, "X", 0, 4, 4, 4));
	dimDescs->push_back(std::make_shared<dimensionDesc>(0, "Y", 0, 4, 4, 4));
	pAttributeDescs attrDescs;
	attrDescs->push_back(std::make_shared<attributeDesc>(0, "A1", eleType::DOUBLE));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "wavelet_test_array", dimDescs, attrDescs);
	pArray sourceArr = std::make_shared<memBlockArray>(arrDesc);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs->at(0), dim, dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->bufferCopy(data, sizeof(data));
	sourceArr->insertChunk(0, sourceChunk);

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

	auto pSet = std::make_shared<wavelet_encode_array_pset>(params);
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
		if (it->getCapacity() == 0)
		{
			throw std::exception();
		}
		for (size_t j = 0; j < it->getCapacity(); ++j, ++i)
		{
			std::cout << (**it).getDouble() << " <> " << expectedOutput[i] << std::endl;
			EXPECT_EQ(ROUNDING((**it).getDouble(), 6), ROUNDING(expectedOutput[i], 6));
			++(*it);
		}
		++cId;
	}
}

namespace caDummy
{
template <typename value_type>
void test_body_wavelet_encode_decode(_pFuncGetSourceArray_, eleDefault wtLevel)
{
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	auto weArr = wavelet_encode(sourceArr, wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "WE Arr" << std::endl;
		weArr->print();
	}

	auto wdArr = wavelet_decode(std::vector<pArray>({ weArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "WD Arr" << std::endl;
		wdArr->print();
	}

	compArrary<value_type>(sourceArr[0], wdArr);
	//EXPECT_TRUE(false);
}
namespace data2D_sc4x4
{
TEST(query_op_wavelet_encode, waveletHaarSimple_sc4x4)
{
	test_body_wavelet_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_wavelet_encode, wavelet_encode_star1024x1024)
{
	test_body_wavelet_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_star1024x1024

namespace data2D_tempTest
{
TEST(query_op_wavelet_encode, wavelet_encode_testTemp)
{
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	auto weArr = wavelet_encode(sourceArr, wtLevel);
	weArr->print();
	auto wdArr = wavelet_decode(std::vector<pArray>({ weArr }), wtLevel);
	wdArr->print();

	compArrary<value_type>(sourceArr[0], wdArr);
}
}	// data2D_tempTest
}	// caDummy
}	// msdb
