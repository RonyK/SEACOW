#pragma once
#ifndef _MSDB_MEMCHUNK_H_
#define _MSDB_MEMCHUNK_H_

#include <array/chunk.h>
#include <array/chunkIterator.h>

namespace msdb
{
class memChunk : public chunk
{
public:
	memChunk(pChunkDesc desc);
	virtual ~memChunk();

protected:
	virtual void makeBuffer();
	virtual void makeBlocks(std::vector<bool> bitmap);
	virtual void referenceBufferToBlock();

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual pBlock getBlock(blockId bId) override;
	virtual blockId getBlockId(pBlockDesc cDesc) override;
	virtual blockId getBlockIdFromItemCoor(coor& itemCoor) override;
	virtual blockId getBlockIdFromBlockCoor(coor& blockCoor) override;
	virtual coor itemCoorToBlockCoor(coor& itemCoor) override;
	virtual pBlockIterator getBlockIterator(iterateMode itMode = iterateMode::ALL) override;

protected:
	pBlock block_;	// memChunk has single block.

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

//////////////////////////////
// Serializable
//////////////////////////////
public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it->isEnd())
		{
			bs << (**it).get<Ty_>();
			++(*it);
		}
	}
	template<class Ty_>
	void deserialize(bstream& bs)
	{
		bs >> setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it->isEnd())
		{
			Ty_ value;
			bs >> value;
			(**it).set<Ty_>(value);
			++(*it);
		}
	}
};
}
#endif	// _MSDB_MEMCHUNK_H_
