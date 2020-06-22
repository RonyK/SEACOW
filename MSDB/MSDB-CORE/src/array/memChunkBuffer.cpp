#include <array/memChunkBuffer.h>
#include <system/exceptions.h>
#include <cassert>

namespace msdb
{
memChunkBuffer::memChunkBuffer()
{

}

memChunkBuffer::~memChunkBuffer()
{
}

bool memChunkBuffer::isCompressed() const
{
	return false;
}

memChunkItemIterator::memChunkItemIterator(void* data, eleType eType, const size_type dSize,
								   position_t* dims, dim_pointer csP)
	:chunkItemIterator(data, eType, dSize, dims, csP)
{}
}
