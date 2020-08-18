#pragma once
#ifndef _MSDB_BLOCKCHUNKBUFFER_H_
#define _MSDB_BLOCKCHUNKBUFFER_H_

#include <array/buffer.h>
#include <array/block.h>
#include <vector>

namespace msdb
{
class blockChunkBuffer;
using pBlockChunkBuffer = std::shared_ptr<blockChunkBuffer>;

class blockChunkBuffer : public buffer
{
public:
	blockChunkBuffer();
	virtual ~blockChunkBuffer();

public:
	// buffer을(를) 통해 상속됨
	virtual void* getData() override;
	virtual void const* getReadData() const override;

	virtual bufferSize size() const override;

	virtual void bufferAlloc(bufferSize size) override;
	virtual void realloc(bufferSize size) override;
	virtual void copy(void* data, bufferSize size) override;
	virtual void linkToChunkBuffer(void* data, bufferSize size);
	virtual void free() override;

protected:
	void* data_;
};
}
#endif	// _MSDB_BLOCKCHUNKBUFFER_H_