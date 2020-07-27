#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <array/blockBuffer.h>
#include <array/blockDesc.h>
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

protected:
	pBlockBuffer cached_;	// hold materialized block
	pBlockDesc desc_;
};
};

#endif