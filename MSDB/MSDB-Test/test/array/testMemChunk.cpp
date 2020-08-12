#include <pch.h>
#include <array/chunk.h>
#include <array/memChunkItemIterator.h>
#include <array/chunkBuffer.h>
#include <array/memChunkBuffer.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc8x8
{
TEST(memChunkBuffer, initRawChunk_sc8x8)
{
	//////////////////////////////
	// Build Dummy Data
	value_type data[dataLength];
	getDummy(data, dataLength);

	memChunkBuffer chunk;
	chunk.bufferAlloc(dataLength);
	chunk.copy(data, dataLength);

	//EXPECT_TRUE(chunk.isAllocated());
	EXPECT_EQ(chunk.size(), dataLength);
}
}
}

TEST(memChunkItemIterator, initRawChunk_sc8x8)
{
	double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
	position_t dim[] = { 2,3,4 };
	position_t csP[] = { 4,5,0 };
	memChunkItemIterator it((void*)data, eleType::DOUBLE, sizeof(dim) / sizeof(int), dim, csP);

	memChunkItemIterator::coordinate_type ceP = it.ceP();
	memChunkItemIterator::coordinate_type sP = it.coorIn2Out();

	for(dimensionId d = 0; d < it.dSize(); d++)
	{
		EXPECT_EQ(dim[d] + csP[d], ceP[d]);
		EXPECT_EQ(csP[d], sP[d]);
	}
}
}