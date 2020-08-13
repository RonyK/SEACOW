#pragma once
#ifndef _MSDB_CHUNKBUFFER_H_
#define _MSDB_CHUNKBUFFER_H_

#include <array/buffer.h>
#include <compression/compression.h>
#include <memory>

namespace msdb
{
class chunkBuffer;
using pChunkBuffer = std::shared_ptr<chunkBuffer>;

class chunkBuffer : public buffer
{
public:
	chunkBuffer();
	virtual ~chunkBuffer();

public:
	virtual void* getData();
	virtual void const* getReadData() const;

	virtual bufferSize size() const;

	virtual void bufferAlloc(bufferSize size);
	virtual void realloc(bufferSize size);
	virtual void copy(void* data, bufferSize size);
	virtual void copy(void* data, bufferSize offset, bufferSize size);
	virtual void linkToChunkBuffer(void* data, bufferSize size);
	virtual void free();
	
protected:
	void* data_;
};
}

#endif		// _MSDB_CHUNKBUFFER_H_
