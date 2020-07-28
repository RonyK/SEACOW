#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <array/chunkItemIterator.h>
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

public:
	virtual void serialize(bstream& os) = 0;
	virtual void deserialize(bstream& is) = 0;

	virtual pChunkItemIterator getItemIterator() = 0;
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;

protected:
	pBlockBuffer cached_;	// hold materialized block
	pBlockDesc desc_;
};
};

#endif