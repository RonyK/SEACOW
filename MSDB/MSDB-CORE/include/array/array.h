#pragma once
#ifndef _MSDB_ARRAY_H_
#define _MSDB_ARRAY_H_

#include <array/arrayDesc.h>
#include <array/chunk.h>
#include <array/chunkIterator.h>
#include <util/coordinate.h>
#include <memory>
#include <map>

namespace msdb
{
template <typename Dty_>
size_t calcNumItems(const Dty_* dims, const size_t dSize)
{
	size_t length = 1;
	for (size_t d = 0; d < dSize; d++)
	{
		length *= dims[d];
	}
	return length;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* chunkDims, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkDims[d]);	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* leafChunkDims, const size_t dSize, const size_t level)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / leafChunkDims[d] / pow(2, level));	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkDims(const Dty_* dims, const Dty_* chunkNums, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkNums[d]);	// ceiling
	}

	return output;
}

class arrayBase;
using pArray = std::shared_ptr<arrayBase>;

class arrayBase : public std::enable_shared_from_this<arrayBase>
{
public:
	using size_type = size_t;
	using chunkContainer = std::map<chunkId, pChunk>;
	using chunkPair = std::pair<chunkId, pChunk>;

public:
	arrayBase(pArrayDesc desc);

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	arrayId getArrayId();
	pArrayDesc getDesc();
	size_type getNumChunks();

	// Chunk
	pChunk getChunk(chunkId cId);
	chunkId getChunkId(pChunkDesc cDesc);
	chunkId getChunkIdFromItemCoor(coor& itemCoor);
	chunkId getChunkIdFromChunkCoor(coor& chunkCoor);
	virtual coor itemCoorToChunkCoor(coor& itemCoor);
	virtual chunkIterator getChunkIterator(
		iterateMode itMode = iterateMode::ALL);

	//////////////////////////////
	// Setter
	//////////////////////////////
	void insertChunk(pChunk inputChunk);
	void flush();

	template <class _Iter>
	void insertChunk(_Iter begin, _Iter end)
	{
		for (; begin != end; ++begin)
		{
			this->chunks_.insert(chunkPair((*begin)->getId(), *begin));
		}
	}

protected:
	pArrayDesc desc_;
	chunkContainer chunks_;		// TODO::Seperate chunk container by attributeId
};
}	// msdb
#endif		// _MSDB_ARRAY_H_