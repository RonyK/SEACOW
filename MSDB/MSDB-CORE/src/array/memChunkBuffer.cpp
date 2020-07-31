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
}
