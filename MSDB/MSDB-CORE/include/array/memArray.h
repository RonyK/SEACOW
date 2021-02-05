#pragma once
#ifndef _MSDB_MEMARRAY_H_
#define _MSDB_MEMARRAY_H_

#include <stdafx.h>
#include <array/array.h>

namespace msdb
{
class memArray : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	memArray(pArrayDesc desc);
	~memArray();

public:
	virtual pChunk makeChunk(const attributeId attrId, const chunkId cId) override;
	virtual pChunk makeChunk(const chunkDesc& desc) override;
	virtual void freeChunk(const chunkId cId) override;
};
}	// msdb
#endif	// _MSDB_MEMARRAY_H_