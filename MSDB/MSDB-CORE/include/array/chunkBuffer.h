#pragma once
#ifndef _MSDB_CHUNKBUFFER_H_
#define _MSDB_CHUNKBUFFER_H_

#include <array/buffer.h>
#include <compression/compression.h>

namespace msdb
{
class chunkBuffer : public buffer
{
public:
	chunkBuffer();
	virtual ~chunkBuffer();

	virtual void* getData();
	virtual void const* getReadData() const;

	virtual bufferSize size() const;

	virtual void alloc(bufferSize size);
	virtual void realloc(bufferSize size);
	virtual void copy(void* data, bufferSize size);
	virtual void assign(void* data, bufferSize size);
	virtual void free();
	
	bool isAllocated();

private:
	bufferSize bodySize_;
	void* data_;
	bool isAllocated_;
};
}

#endif		// _MSDB_CHUNKBUFFER_H_
