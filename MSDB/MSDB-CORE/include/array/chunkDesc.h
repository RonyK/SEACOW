#pragma once
#ifndef _MSDB_CHUNKDESC_H_
#define _MSDB_CHUNKDESC_H_

#include <array/chunkId.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <compression/compression.h>
#include <util/coordinate.h>
#include <array/buffer.h>
#include <memory>

namespace msdb
{
class chunkDesc;

using pChunkDesc = std::shared_ptr<chunkDesc>;
using chunkSize = bufferSize;
extern const chunkSize INVALID_CHUNK_SIZE;

class chunkDesc : std::enable_shared_from_this<chunkDesc>
{	
	struct chunkHeader
	{
		uint64_t bytes;

	};
public:
	chunkDesc();

	chunkDesc(const chunkId id,
			  pAttributeDesc attrDesc, const dimension& dims, 
			  const coor sp, const coor ep, const chunkSize mSize = INVALID_CHUNK_SIZE);

	chunkDesc(const chunkId id, 
			  pAttributeDesc attrDesc, const dimension& dims,
			  const coor sp, const coor ep, 
			  const chunkSize mSize, const chunkSize cSize,
			  const CompressionMethod cType);

public:
	void setDim(dimensionId dId, position_t value);

public:
	void initPhysicalChunkSizeFromDims();

public:
	chunkId id_;				// chunk id
	pAttributeDesc attrDesc_;	// attribute desc

	chunkSize mSize_;			// materialized size
	chunkSize cSize_;			// compressed size,
								// if not: cSize == mSize or INVALID_CHUNK_SIZE
	bool useCompression_;		// can be compressed
	CompressionMethod cType_;	// compressed method type
								// if not: CompressionMethod::NONE

	dimension dims_;			// dimensions
	coor sp_;					// start point
	coor ep_;					// end point
};
}
#endif		// _MSDB_CHUNKDESC_H_
