#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <memory>
#include <array/blockDesc.h>
#include <array/blockBuffer.h>

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

protected:
	blockBuffer* cached_;	// hold materialized block
	pBlockDesc desc_;
};
};

#endif