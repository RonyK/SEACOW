#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

#include <array/chunkBuffer.h>
#include <array/chunkDesc.h>
#include <memory>

namespace msdb
{
class chunk;
using pChunk = std::shared_ptr<chunk>;

class chunk : public std::enable_shared_from_this<chunk>
{
public:
	chunk(chunkDesc* desc);
	bool isMaterialized() const;

private:
	chunkBuffer* cached_;	// hold materialized chunk
	chunkDesc desc_;		// chunk desc
};
};
#endif
