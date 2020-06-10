#pragma once
#ifndef _MSDB_CHUNKDESC_H_
#define _MSDB_CHUNKDESC_H_

#include <array/chunkId.h>
#include <array/attributeId.h>
#include <compression/compression.h>
#include <util/coordinate.h>
#include <array/buffer.h>

namespace msdb
{
using chunkSize = bufferSize;

class chunkDesc
{	
	struct chunkHeader
	{
		size_t bytes;

	};
public:
	chunkDesc();

	chunkDesc(attributeId id, coor sp, coor ep, chunkSize mSize);

	chunkDesc(attributeId id, coor sp, coor ep, chunkSize mSize, chunkSize cSize, CompressionMethod cType);

	attributeId id_;		// attribute ID
	chunkSize mSize_;		// materialized size
	chunkSize cSize_;		// compressed size
	bool useCompression_;	// can be compressed
	CompressionMethod cType_;
	coor sp_;				// start point
	coor ep_;				// end point
};
}
#endif		// _MSDB_CHUNKDESC_H_
