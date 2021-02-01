#include <stdafx.h>
#include <array/memChunkBuffer.h>
#include <system/exceptions.h>

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
