#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

#include <array/chunkBuffer.h>
#include <array/chunkDesc.h>
#include <memory>

namespace msdb
{
class chunk;
class chunkItemIterator;
using pChunk = std::shared_ptr<chunk>;

class chunk : public std::enable_shared_from_this<chunk>
{
public:
	using size_type = size_t;

public:
	chunk(pChunkDesc desc);
	virtual ~chunk();

public:
	void materialize();
	void materialize(bufferSize size);
	void materializeCopy(void* data, bufferSize size);
	void materializeAssign(void* data, bufferSize size);
	bool isMaterialized() const;

	chunkItemIterator getItemIterator();
	chunkId getId() const;
	const pChunkDesc getDesc() const;
	size_type numCells();
	
protected:
	void free();
	void makeBuffer();

protected:
	chunkBuffer* cached_;	// hold materialized chunk
	pChunkDesc desc_;		// chunk desc
};

class chunkItemIterator : public itemItr
{
public:
	using self_type = chunkItemIterator;
	using base_type = itemItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	chunkItemIterator(void* data, eleType eType, const size_type dSize,
					  position_t* dims, dim_pointer csP);

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
};
#endif
