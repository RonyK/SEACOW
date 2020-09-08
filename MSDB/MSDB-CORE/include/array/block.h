#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <array/blockItemIterator.h>
#include <array/blockBuffer.h>
#include <array/blockDesc.h>
#include <index/bitmap.h>
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
	void setIsp(coor isp);
	void setIep(coor iep);
	coorRange getBlockRange();
	coorRange getBlockItemRange();

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
	virtual void refChunkBufferWithoutOwnership(void* data, const bufferSize size) = 0;	// used in chunk
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

	void copyBitmap(cpBitmap itemBitmap);
	void replaceBitmap(pBitmap itemBitmap);
	void mergeBitmap(pBitmap itemBitmap);

protected:
	pBitmap itemBitmap_;

//////////////////////////////
// Print
//////////////////////////////
public:
	void print();

protected:
	template <class Ty_>
	void printImp()
	{
		auto iit = this->getItemIterator();
		while(!iit->isEnd())
		{
			if(iit->isExist())
			{
				std::cout << (**iit).get<Ty_>() << ", ";
			}else
			{
				std::cout << "*, ";
			}
			++(*iit);
		}
		std::cout << std::endl;
	}
	template<>
	void printImp<char>()
	{
		auto iit = this->getItemIterator();
		while (!iit->isEnd())
		{
			if (iit->isExist())
			{
				std::cout << static_cast<int>((**iit).get<char>()) << ", ";
			} else
			{
				std::cout << "*, ";
			}
			++(*iit);
		}
		std::cout << std::endl;
	}
};
};

#endif