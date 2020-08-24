#pragma once
#ifndef _MSDB_MEMBLOCKARRAY_H_
#define _MSDB_MEMBLOCKARRAY_H_

#include <array/array.h>

namespace msdb
{
class memBlockArray : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	memBlockArray(pArrayDesc desc);
	~memBlockArray();

public:
	virtual pChunk makeChunk(const attributeId attrId, const chunkId cId) override;
	virtual pChunk makeChunk(const chunkDesc& desc) override;
};
}		// msdb
#endif	// _MSDB_MEMBLOCKARRAY_H_