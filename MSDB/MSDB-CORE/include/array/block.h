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
class chunk;
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
	dimensionId getDSize();

	virtual void serialize(bstream& os) = 0;
	virtual void deserialize(bstream& is) = 0;

protected:
	pBlockDesc desc_;

//////////////////////////////
// Buffer
//////////////////////////////
public:
	virtual void copy(pBlock sBlock);
	bool isMaterialized() const;

	// Shallow copy
	// A block buffer only references a part of a chunk buffer memory.
	virtual void unlinkFromChunkBuffer();
	virtual void linkToChunkBuffer(void* data, const bufferSize size) = 0;	// used in chunk
protected:
	pBlockBuffer getBuffer();

	friend class chunk;

protected:
	pBlockBuffer cached_;	// hold materialized block
	
//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pBlockItemIterator getItemIterator() = 0;
	virtual pBlockItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;

//////////////////////////////
// Print
//////////////////////////////
public:
	void print();

protected:
	template <class Ty_>
	void printImp()
	{
		auto it = this->getItemIterator();
		std::cout << "==============================" << std::endl;
		for (size_t i = 0; i < it->getCapacity() && !it->isEnd(); ++i, ++(*it))
		{
			std::cout << (**it).get<Ty_>() << ", ";
		}
		std::cout << std::endl << "==============================" << std::endl;
	}
	template<>
	void printImp<char>()
	{
		auto it = this->getItemIterator();
		std::cout << "==============================" << std::endl;
		for (size_t i = 0; i < it->getCapacity() && !it->isEnd(); ++i, ++(*it))
		{
			std::cout << static_cast<int>((**it).get<char>()) << ", ";
		}
		std::cout << std::endl << "==============================" << std::endl;
	}
};
};

#endif