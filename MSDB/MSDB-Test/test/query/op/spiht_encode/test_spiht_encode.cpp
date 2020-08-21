#include <pch.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <dummy/spihtDummy.h>
#include <compression/testCompression.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_spiht_encode, spiht_encode_sc4x4)
{
	// Assing new array id for se compressed array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 3);     // 444
	std::cout << "##############################" << std::endl;
	std::cout << "Source Arr" << std::endl;
	sourceArr[0]->print();

	auto arr_spiht_encode = spiht_encode(sourceArr);
	std::cout << "##############################" << std::endl;
	std::cout << "Spiht Encoded Arr" << std::endl;
}
}	// data2D_sc4x4

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
	arrayId aid = 99445;
	pDimensionDescs dimDescs;
	dimDescs->push_back(std::make_shared<dimensionDesc>(0, "X", 0, 4, 4, 4));
	dimDescs->push_back(std::make_shared<dimensionDesc>(0, "Y", 0, 4, 4, 4));
	pAttributeDescs attrDescs;
	attrDescs->push_back(std::make_shared<attributeDesc>(0, "A1", eleType::CHAR));
		
	pArrayDesc arrDesc = std::make_shared<arrayDesc>(aid, "spiht_test_array", dimDescs, attrDescs);
	auto sourceArr = std::make_shared<wavelet_encode_array>(arrDesc);
	sourceArr->setMaxLevel(0);
	sourceArr->setOrigianlChunkDims(chunkDims);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs->at(0), dim, dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->makeAllBlocks();
	sourceChunk->bufferCopy(data, sizeof(data));
	sourceArr->insertChunk(0, sourceChunk);

	//////////////////////////////
	// Encoding
	pQuery q = std::make_shared<query>();
	auto spihtPlan = std::make_shared<spiht_encode_plan>();
	auto spihtAction = std::make_shared<spiht_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(arrDesc)
	};

	auto pSet = std::make_shared<spiht_encode_array_pset>(params);
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
	arrayId aid = 99885;
	pDimensionDescs dimDescs = std::make_shared<dimensionDescs>();
	dimDescs->push_back(std::make_shared<dimensionDesc>(1, "X", 0, 8, 8, 8));
	dimDescs->push_back(std::make_shared<dimensionDesc>(1, "Y", 0, 8, 8, 8));
	pAttributeDescs attrDescs = std::make_shared<attributeDescs>();
	attrDescs->push_back(std::make_shared<attributeDesc>(1, "A1", eleType::CHAR));

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(aid, "spiht_test_array", dimDescs, attrDescs);
	auto sourceArr = std::make_shared<wavelet_encode_array>(arrDesc);
	sourceArr->setMaxLevel(0);
	sourceArr->setOrigianlChunkDims(chunkDims);
	std::vector<pArray> arrs = { sourceArr };

	pChunkDesc cDesc = std::make_shared<chunkDesc>(0, attrDescs->at(0), dim, dim, sP, eP);
	pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
	sourceChunk->bufferCopy(data, sizeof(data));
	sourceChunk->makeAllBlocks();
	sourceArr->insertChunk(0, sourceChunk);

	//////////////////////////////
	// Encoding
	pQuery q = std::make_shared<query>();
	auto spihtPlan = std::make_shared<spiht_encode_plan>();
	auto spihtAction = std::make_shared<spiht_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(arrDesc)
	};

	auto pSet = std::make_shared<spiht_encode_array_pset>(params);
	spihtPlan->setParamSet(pSet);

	spihtAction->setArrayDesc(spihtPlan->inferSchema());
	spihtAction->setParams(params);
	auto weArray = spihtAction->execute(arrs, q);

	//EXPECT_EQ(expect, code);
}
}
}
}
