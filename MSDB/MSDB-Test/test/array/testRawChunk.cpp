#include <pch.h>
#include <array/chunk.h>
#include <array/chunkBuffer.h>
#include <array/memChunk.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc8x8
{
TEST(memChunk, initRawChunk_sc8x8)
{
	//////////////////////////////
	// Build Dummy Data
	value_type data[dataLength];
	getDummy(data, dataLength);

	memChunk chunk;
	chunk.alloc(dataLength);
	chunk.copy(data, dataLength);

	EXPECT_TRUE(chunk.isAllocated());
	EXPECT_EQ(chunk.size(), dataLength);
}
}
}

}