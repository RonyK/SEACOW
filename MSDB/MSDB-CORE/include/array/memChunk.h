#pragma once
#ifndef _MSDB_RAWCHUNK_H_
#define _MSDB_RAWCHUNK_H_

#include <array/chunk.h>
#include <util/coordinate.h>

namespace msdb
{
class memChunk : public chunkBuffer
{
public:
	memChunk();
	virtual ~memChunk();

public:
	virtual bool isCompressed() const;
};

class memChunkItemIterator : public chunkItemIterator
{
public:
	using self_type = memChunkItemIterator;
	using base_type = chunkItemIterator;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	memChunkItemIterator(void* data, eleType eType, const size_type dSize, position_t* dims,
					 dim_pointer csP);
};
}

#endif		// _MSDB_RAWCHUNK_H_
