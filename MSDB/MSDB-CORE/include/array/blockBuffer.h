#pragma once
#ifndef _MSDB_BLOCKBUFFER_H_
#define _MSDB_BLOCKBUFFER_H_

#include <array/chunkBuffer.h>

namespace msdb
{
class blockBuffer;
using pBlockBuffer = std::shared_ptr<blockBuffer>;

// Note: block buffer only holds a pointer to a chunk buffer memory.
class blockBuffer : public chunkBuffer
{
public:
	blockBuffer(void* data, bufferSize size);
	virtual ~blockBuffer();
};

class memBlockBuffer : public blockBuffer
{
public:
	memBlockBuffer(void* data, bufferSize size);
	virtual ~memBlockBuffer();
};
}	// msdb
#endif		// _MSDB_BLOCKBUFFER_H_
