#pragma once
#ifndef _MSDB_RAWCHUNK_H_
#define _MSDB_RAWCHUNK_H_

#include <stdafx.h>
#include <array/chunk.h>
#include <array/chunkBuffer.h>
#include <util/coordinate.h>

namespace msdb
{
class memChunkBuffer : public chunkBuffer
{
public:
	memChunkBuffer();
	virtual ~memChunkBuffer();

public:
	virtual bool isCompressed() const;
};
}

#endif		// _MSDB_RAWCHUNK_H_
