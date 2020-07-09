#pragma once
#ifndef _MSDB_WT_CHUNK_H_
#define _MSDB_WT_CHUNK_H_

#include <array/chunk.h>

namespace msdb
{
class wtChunk;
using pWtChunk = std::shared_ptr<wtChunk>;

class wtChunk : public chunk
{
public:
	wtChunk(pChunkDesc desc);
	virtual ~wtChunk();

public:
	size_t getLevel();
	chunkId getBandId();
	chunkId getSourceChunkId();

	void setLevel(size_t level);
	void setBandId(chunkId bid);
	void setSourceChunkId(chunkId cid);

protected:
	size_t level_;
	chunkId bandId_;
	chunkId sourceChunkId;
};
}	// msdb
#endif		// _MSDB_WT_CHUNK_H_