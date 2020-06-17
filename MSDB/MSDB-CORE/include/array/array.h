#pragma once
#ifndef _MSDB_ARRAY_H_
#define _MSDB_ARRAY_H_

#include <array/arrayDesc.h>
#include <array/chunk.h>
#include <util/coordinate.h>
#include <memory>
#include <map>

namespace msdb
{
template <typename Dty_>
size_t calcArrayCellNums(const Dty_* dims, const size_t dSize)
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
class chunkIterator;
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
	pArrayDesc getDesc();
	virtual chunkIterator getIterator();
	size_type getNumChunks();

	virtual coor itemCoorToChunkCoor(coor& itemCoor);
	void insertChunk(pChunk inputChunk);
	template <class _Iter>
	void insertChunk(_Iter begin, _Iter end)
	{
		for (; begin != end; ++begin)
		{
			this->chunks_.insert(chunkPair((*begin)->getId(), *begin));
		}
	}
	pChunk getChunk(chunkId cId);

	virtual chunkId getChunkIdFromItemCoor(coor& itemCoor);
	virtual chunkId getChunkIdFromChunkCoor(coor& chunkCoor);

protected:
	pArrayDesc desc_;
	chunkContainer chunks_;
};

class chunkIterator : public coorItr
{
public:
	using self_type = chunkIterator;
	using size_type = coorItr::size_type;
	using chunkContainer = arrayBase::chunkContainer;

public:
	chunkIterator(const size_type dSize, dim_const_pointer dims, 
				  chunkContainer* chunks);

	chunkIterator(const self_type& mit);

public:
	size_type getSeqEnd();

	pChunk operator*() { return this->chunks_->at(this->seqPos_); }
	pChunk operator->() { return this->chunks_->at(this->seqPos_); }

protected:
	chunkContainer* chunks_;
};
}

#endif