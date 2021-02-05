#pragma once
#ifndef _MSDB_MEMBLOCK_H_
#define _MSDB_MEMBLOCK_H_

#include <stdafx.h>
#include <array/block.h>
#include <array/chunkIterator.h>
#include <array/chunkItemIterator.h>
#include <io/bitstream.h>

namespace msdb
{
class memBlock : public block
{
public:
	memBlock(pBlockDesc desc);
	virtual ~memBlock();

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pBlockItemIterator getItemIterator();
	virtual pBlockItemRangeIterator getItemRangeIterator(const coorRange& range);

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void refChunkBufferWithoutOwnership(void* data, bufferSize size) override;

//////////////////////////////
// Serializable
//////////////////////////////
public:
	virtual void serialize(bstream& os) override;
	virtual void deserialize(bstream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();
		while (!it->isEnd())
		{
			bs << (**it).get<Ty_>();
			++(*it);
		}
	}
	template<typename Ty_>
	void deserializeTy(bstream& bs)
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
}	// msdb

#endif