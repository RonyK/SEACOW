#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <array/blockItemIterator.h>
#include <array/blockBuffer.h>
#include <array/blockDesc.h>
#include <io/bitstream.h>
#include <memory>

namespace msdb
{
class block;
class blockItemIterator;
using pBlock = std::shared_ptr<block>;

class block : public std::enable_shared_from_this<block>
{
public:
	block(pBlockDesc desc);
	virtual ~block();

//////////////////////////////
// Desc
//////////////////////////////
public:
	blockId getId();
	pBlockDesc getDesc();

	virtual void serialize(bstream& os) = 0;
	virtual void deserialize(bstream& is) = 0;

protected:
	pBlockDesc desc_;

//////////////////////////////
// Buffer
//////////////////////////////
public:
	// Shallow copy
	// A block buffer only references a part of a chunk buffer memory.
	virtual void unreference();
	virtual void reference(void* data, const bufferSize size) = 0;
	bool isMaterialized() const;

protected:
	pBlockBuffer cached_;	// hold materialized block
	
//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pBlockItemIterator getItemIterator() = 0;
	virtual pBlockItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;
};
};

#endif