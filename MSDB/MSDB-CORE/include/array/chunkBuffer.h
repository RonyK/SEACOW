#pragma once
#ifndef _MSDB_CHUNKBUFFER_H_
#define _MSDB_CHUNKBUFFER_H_

#include <array/buffer.h>

namespace msdb
{
class chunkBuffer : public buffer
{
public:
	chunkBuffer();
	~chunkBuffer();

	virtual void* getData();
	virtual void const* getReadData() const;

	virtual bufferSize size() const;

	virtual void alloc(bufferSize size);
	virtual void realloc(bufferSize size);
	virtual void copy(void* data, bufferSize size);
	virtual void free();
	
	bool isAllocated();

	virtual void compress();
	virtual void uncompress();

private:
	bufferSize size_;
	void* data_;
	bool isAllocated_;
};
}

#endif		// _MSDB_CHUNKBUFFER_H_
