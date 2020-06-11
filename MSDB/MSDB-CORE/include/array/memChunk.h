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

public:
	virtual bool isCompressed() const;
};

class memChunkIterator : public itemItr
{
public:
	using self_type = memChunkIterator;
	using base_type = itemItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	memChunkIterator(void* data, eleType eType, const size_type dSize, position_t* dims,
					 dim_pointer csP);

public:
	coordinate_type coorOut2In(coordinate_type& out);
	coordinate_type coorIn2Out(coordinate_type& in);
	coordinate_type coorIn2Out();
	coordinate_type ceP();

	coordinate_type outCoor();
	coordinate_type innerCoor();

private:
	dim_pointer csP_;			// Chunk start point
};
}

#endif		// _MSDB_RAWCHUNK_H_
