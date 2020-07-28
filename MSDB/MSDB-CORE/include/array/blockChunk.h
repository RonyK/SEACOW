#pragma once
#ifndef _MSDB_BLOCKEDCHUNK_H_
#define _MSDB_BLOCKEDCHUNK_H_

#include <array/block.h>
#include <array/blockIterator.h>
#include <array/blockContainer.h>
#include <array/chunkIterator.h>
#include <util/coordinate.h>
#include <map>

namespace msdb
{
std::map<int, int> ma;
auto a = ma.begin();
class blockIterator;

class blockChunk : public chunk
{
public:
	using size_type = chunk::size_type;

public:
	blockChunk(pChunkDesc desc);
	virtual ~blockChunk();

protected:
	virtual void makeBuffer();

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	// Block
	pBlock getBlock(blockId bId);
	blockId getBlockId(pBlockDesc cDesc);
	blockId getBlockIdFromItemCoor(coor& itemCoor);
	blockId getBlockIdFromBlockCoor(coor& chunkCoor);
	virtual coor itemCoorToBlockCoor(coor& itemCoor);
	virtual blockIterator getBlockIterator(iterateMode itMode = iterateMode::ALL);
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

	//////////////////////////////
	// Setter
	//////////////////////////////
	void flush();

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

protected:
	blockContainer blocks_;
};

class blockChunkItemIterator : public chunkItemIterator
{
public:
	using self_type = blockChunkItemIterator;
	using base_type = chunkItemIterator;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	// csP : chunk start point
	blockChunkItemIterator(void* data, const eleType eType, const size_type dSize, 
						   dim_const_pointer dims,
						   dim_const_pointer csP, blockIterator bItr);

	blockChunkItemIterator(void* data, const eleType eType,
						   const dimension dims,
						   const dimension csP, blockIterator bItr);

protected:
	blockIterator bItr_;
	std::shared_ptr<chunkItemIterator> curBlockItemItr_;
};

class blockChunkItemRangeIterator : public chunkItemRangeIterator
{
public:

	blockChunkItemRangeIterator(void* data, const eleType eType, const size_type dSize,
								dim_const_pointer dims,
								dim_const_pointer sP, dim_const_pointer eP,
								dim_const_pointer csP, blockIterator bItr);

	blockChunkItemRangeIterator(void* data, eleType eType, 
								const dimension dims,
								const coorRange& range,
								const dimension csP, blockIterator bItr);
	
protected:
	blockIterator bItr_;
};
}
#endif		// _MSDB_BLOCKEDCHUNK_H_