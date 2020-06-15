#pragma once
#ifndef _MSDB_ARRAY_H_
#define _MSDB_ARRAY_H_

#include <array/arrayDesc.h>
#include <array/chunk.h>
#include <util/coordinate.h>
#include <memory>

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
class arrayIterator;
using pArray = std::shared_ptr<arrayBase>;

class arrayBase : public std::enable_shared_from_this<arrayBase>
{
public:
	arrayBase(arrayDesc& desc);

public:
	arrayDesc getDesc();
	virtual arrayIterator getIterator();


private:
	arrayDesc desc_;
	std::vector<pChunk> chunks_;
};

class arrayIterator : public coorItr
{
public:
	using self_type = arrayIterator;

public:
	arrayIterator(const size_type dSize, dim_const_pointer dims);

	arrayIterator(const self_type& mit);
};
}

#endif