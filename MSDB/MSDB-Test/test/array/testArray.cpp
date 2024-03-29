#include <pch_test.h>
#include <array/memBlockArray.h>

namespace msdb
{
namespace core
{
TEST(arrayBase, memBlockArrayCreate)
{
	//////////////////////////////
	// Set up array
	pDimensionDesc dimDesc = std::make_shared<dimensionDesc>(0, "x", 0, 8, 8, 8);
	pAttributeDesc attrDesc = std::make_shared<attributeDesc>(0, "a1", eleType::CHAR);

	pDimensionDescs dimDescs = std::make_shared<dimensionDescs>();
	dimDescs->push_back(dimDesc);
	pAttributeDescs attrDescs = std::make_shared<attributeDescs>();
	attrDescs->push_back(attrDesc);

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(0, "wavelet_test", dimDescs, attrDescs);

	pArray sourceArr = std::make_shared<memBlockArray>(arrDesc);
}
}		// core
}		// msdb