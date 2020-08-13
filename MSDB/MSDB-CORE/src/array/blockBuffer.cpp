#include <array/blockBuffer.h>

namespace msdb
{
blockBuffer::blockBuffer(void* data, bufferSize size)
	: chunkBuffer()
{
	this->linkToChunkBuffer(data, size);
}

blockBuffer::~blockBuffer()
{
}
memBlockBuffer::memBlockBuffer(void* data, bufferSize size)
	: blockBuffer(data, size)
{
}
memBlockBuffer::~memBlockBuffer()
{
}
}