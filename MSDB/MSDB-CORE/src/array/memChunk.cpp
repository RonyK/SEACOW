#include <array/memChunk.h>
#include <system/exceptions.h>
#include <cassert>

namespace msdb
{
memChunk::memChunk()
{

}

memChunk::~memChunk()
{
}

bool memChunk::isCompressed() const
{
	return false;
}

memChunkItemIterator::memChunkItemIterator(void* data, eleType eType, const size_type dSize,
								   position_t* dims, dim_pointer csP)
	:chunkItemIterator(data, eType, dSize, dims, csP)
{}
}
