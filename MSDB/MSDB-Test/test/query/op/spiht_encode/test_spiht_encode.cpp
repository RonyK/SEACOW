#include <pch.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <compression/testSPIHT.h>
#include <dummy/spihtDummy.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_spiht_sc4x4
{
TEST(query_op_spiht_encode, spiht_2D_4x4)
{
	//////////////////////////////
	// Set up dummy data
	char data[][4] = { {26,6,13,10}, {-7,7,6,4}, {4,-4,4,-3}, {2,-2,-2,0} };
	std::list<int> expect = { 0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,
							 1,0,0,0,0,0,0,0,
							 0,0,0,1,1,0,1,0,0,0,0,0,1,
							 1,0,1,1,1,0,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,0,0,1,0,
							 1,0,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,
							 0,0,0,1,0 };
	dimension dim = { sizeof(data) / sizeof(char) / 4, 4 };
	coor sP = { 0, 0 };
	coor eP = { 4, 4 };


	//////////////////////////////
	// Set up array
	dimensionDescs dimDescs;
	dimDescs.push_back(std::make_shared<dimensionDesc>(0, "X", 0, 4, 4));
	dimDescs.push_back(std::make_shared<dimensionDesc>(0, "Y", 0, 4, 4));
	attributeDescs attrDescs;
	attrDescs.push_back(std::make_shared<attributeDesc>(0, "A1", eleType::CHAR));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "spiht_test_array", dimDescs, attrDescs);
	auto sourceArr = std::make_shared<wavelet_encode_array>(arrDesc, 0);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs[0], dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->materializeCopy(data, sizeof(data));
	sourceArr->insertChunk(sourceChunk);

	//////////////////////////////
	// Encoding
	pQuery q = std::make_shared<query>();
	auto spihtPlan = std::make_shared<spiht_encode_plan>();
	auto spihtAction = std::make_shared<spiht_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(arrDesc)
	};

	auto pSet = std::make_shared<spiht_encode_pset>(params);
	spihtPlan->setParamSet(pSet);

	spihtAction->setArrayDesc(spihtPlan->inferSchema());
	spihtAction->setParams(params);
	auto weArray = spihtAction->execute(arrs, q);
}

TEST(query_op_spiht_encode, spiht_2D_8x8)
{
	//////////////////////////////
	// Set up dummy data
	char data[][8] = { {113,-81,17,1,17,-1,1,1}, {-49,81,1,33,1,17,1,1}, {33,1,1,1,1,1,1,17}, {81,1,1,-17,1,1,-49,1},
					   {-1,1,1,1,1,1,1,1}, {1,-1,1,1,1,1,1,1}, {1,1,1,1,1,1,-1,1}, {1,1,1,1,1,1,1,-1} };
	std::list<int> expect = { 1,0,1,1,0,1,0,0,1,0,0,1,0,0,0,0,
							 1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,0,0,0,
							 0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1 };
	dimension dim = { sizeof(data) / sizeof(char) / 8, 8 };
	coor sP = { 0, 0 };
	coor eP = { 8, 8 };


	//////////////////////////////
	// Set up array
	dimensionDescs dimDescs;
	dimDescs.push_back(std::make_shared<dimensionDesc>(1, "X", 0, 8, 8));
	dimDescs.push_back(std::make_shared<dimensionDesc>(1, "Y", 0, 8, 8));
	attributeDescs attrDescs;
	attrDescs.push_back(std::make_shared<attributeDesc>(1, "A1", eleType::CHAR));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(1, "spiht_test_array", dimDescs, attrDescs);
	auto sourceArr = std::make_shared<wavelet_encode_array>(arrDesc, 0);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs[0], dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->materializeCopy(data, sizeof(data));
	sourceArr->insertChunk(sourceChunk);

	//////////////////////////////
	// Encoding
	pQuery q = std::make_shared<query>();
	auto spihtPlan = std::make_shared<spiht_encode_plan>();
	auto spihtAction = std::make_shared<spiht_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(arrDesc)
	};

	auto pSet = std::make_shared<spiht_encode_pset>(params);
	spihtPlan->setParamSet(pSet);

	spihtAction->setArrayDesc(spihtPlan->inferSchema());
	spihtAction->setParams(params);
	auto weArray = spihtAction->execute(arrs, q);

	//EXPECT_EQ(expect, code);
}
}
}
}
