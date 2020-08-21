#pragma once
#ifndef _MSDB_BLOCK_DESC_H_
#define _MSDB_BLOCK_DESC_H_

#include <array/buffer.h>
#include <array/blockId.h>
#include <array/dimensionDesc.h>
#include <util/coordinate.h>
#include <memory>

namespace msdb
{
class blockDesc;

using pBlockDesc = std::shared_ptr<blockDesc>;
using blockSize = bufferSize;
extern const blockSize INVALID_BLOCK_SIZE;

class blockDesc : std::enable_shared_from_this<blockDesc>
{
public:
	blockDesc();

	blockDesc(const blockId id, const eleType eType,
			  const dimension& dims,
			  const coor& sp, const coor& ep, 
			  const blockSize mSize = INVALID_BLOCK_SIZE,
			  const blockSize mOffset = INVALID_BLOCK_SIZE);

protected:
	void initBlockCoor();

public:
	blockId id_;				// in chunk
	blockSize mSize_;			// materialized size
	blockSize mOffset_;			// memory offset in chunkBuffer
	eleType eType_;

	dimension dims_;			// dimensions
	coor sp_;					// in chunk
	coor ep_;					// in chunk
	coor blockCoor_;			// in chunk
};
}
#endif		// _MSDB_BLOCK_DESC_H_