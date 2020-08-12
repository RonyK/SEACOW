#pragma once
#ifndef _MSDB_BLOCKEDCHUNK_H_
#define _MSDB_BLOCKEDCHUNK_H_

#include <array/chunkIterator.h>
#include <array/memChunkItemIterator.h>
#include <util/coordinate.h>
#include <map>

namespace msdb
{
class blockIterator;

class memBlockChunk : public chunk
{
public:
	using size_type = chunk::size_type;

public:
	memBlockChunk(pChunkDesc desc);
	virtual ~memBlockChunk();

protected:
	virtual void makeBuffer();
	virtual void makeBlocks(std::vector<bool> bitmap);
	virtual void referenceBufferToBlock();

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	// Block
	virtual pBlock getBlock(blockId bId);
	virtual blockId getBlockId(pBlockDesc cDesc);
	virtual blockId getBlockIdFromItemCoor(coor& itemCoor);
	virtual blockId getBlockIdFromBlockCoor(coor& chunkCoor);
	virtual virtual coor itemCoorToBlockCoor(coor& itemCoor);
	virtual pBlockIterator getBlockIterator(iterateMode itMode = iterateMode::ALL);

	//////////////////////////////
	// Setter
	//////////////////////////////
	void setBlock(pBlock b);
	void flush();

	//////////////////////////////
	// Item Iterators
	//////////////////////////////
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

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
	blockChunkItemIterator(void* data, const eleType eType,
						   const dimension& dims,
						   const dimension& csP,
						   pBlockIterator bItr);

public:
	virtual void next() override;
	virtual void prev() override;

	virtual element getAt(position_t pos) override;
	virtual element operator*() override;

protected:
	void initBlockItemItr();
	pBlockIterator bItr_;
	pBlockItemIterator curBlockItemItr_;
};

class blockChunkItemRangeIterator : public chunkItemRangeIterator
{
public:
	blockChunkItemRangeIterator(void* data, eleType eType, 
								const dimension& dims,
								const coorRange& range,
								const dimension& csP, 
								pBlockIterator bItr);

public:
	virtual void next() override;
	virtual void prev() override;

	virtual element getAt(position_t pos) override;
	virtual element operator*() override;

protected:
	pBlockIterator bItr_;
	pBlockItemRangeIterator curBlockItemItr_;
};
}
#endif		// _MSDB_BLOCKEDCHUNK_H_