#include <pch.h>
#include <array/array.h>

namespace msdb
{
TEST(arrayBase, arrayCreate)
{
	//////////////////////////////
	// Set up array
	pDimensionDesc dimDesc = std::make_shared<dimensionDesc>(0, "x", 0, 8, 8, 8);
	pAttributeDesc attrDesc = std::make_shared<attributeDesc>(0, "a1", eleType::DOUBLE);

	dimensionDescs dimDescs;
	dimDescs.push_back(dimDesc);
	attributeDescs attrDescs;
	attrDescs.push_back(attrDesc);

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "wavelet_test", dimDescs, attrDescs);

	pArray sourceArr = std::make_shared<arrayBase>(arrDesc);
}
}
