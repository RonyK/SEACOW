#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

#include <array/chunkBuffer.h>
#include <array/chunkDesc.h>
#include <array/chunkItemIterator.h>
#include <array/block.h>
#include <array/blockIterator.h>
#include <array/blockContainer.h>
#include <io/serializable.h>
#include <io/bitstream.h>
#include <index/bitmap.h>
#include <memory>

namespace msdb
{
class chunk;
using pChunk = std::shared_ptr<chunk>;

class chunk : public std::enable_shared_from_this<chunk>, public serializable
{
public:
	using size_type = size_t;

public:
	chunk(pChunkDesc desc);
	virtual ~chunk();

//////////////////////////////
// Desc
//////////////////////////////
public:
	chunkId getId() const;
	void setId(chunkId id);	// chunk id can be chnaged in query processing
	const pChunkDesc getDesc() const;
	size_type getDSize();
	size_type numCells();
	coor getChunkCoor();

protected:
	pChunkDesc desc_;		// chunk desc

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	void free();
	// NOTE::Call a referenceBufferToBlock() in a makeBuffer()
	virtual void makeBuffer() = 0;
	virtual void referenceBufferToBlock(const blockId bId) = 0;
	virtual void referenceAllBufferToBlock();

public:
	virtual void bufferAlloc();
	virtual void bufferAlloc(bufferSize size);
	virtual void bufferCopy(void* data, bufferSize size);
	virtual void bufferCopy(pChunk source);
	virtual void bufferCopy(pBlock source);
	virtual void bufferRef(void* data, bufferSize size);
	virtual void bufferRef(pChunk source);
	bool isMaterialized() const;

public:
	bool operator == (const chunk& mit)
	{
		if(this->getId() != mit.getId())
		{
			return false;
		}

		// TODO::Compare desc
		if(this->desc_->mSize_ != mit.getDesc()->mSize_)
		{
			return false;
		}
		
		if(memcmp(this->cached_->getReadData(), mit.cached_->getReadData(), this->desc_->mSize_))
		{
			//auto lbItr = (*lcItr)->getBlockIterator();
			//auto rbItr = (*rcItr)->getBlockIterator();

			//while (!lbItr->isEnd() && !rbItr->isEnd())
			//{
			//	auto liItr = (*lbItr)->getItemIterator();
			//	auto riItr = (*rbItr)->getItemIterator();

			//	while (!liItr->isEnd() && !riItr->isEnd())
			//	{
			//		Ty_ li = (**liItr).get<Ty_>();
			//		Ty_ ri = (**riItr).get<Ty_>();

			//		EXPECT_EQ(li, ri);

			//		++(*liItr);
			//		++(*riItr);
			//	}

			//	EXPECT_EQ(liItr->isEnd(), riItr->isEnd());

			//	++(*lbItr);
			//	++(*rbItr);
			//}

			//EXPECT_EQ(lbItr->isEnd(), rbItr->isEnd());
			return false;
		}

		return true;
	}

protected:
	pChunkBuffer getBuffer();

protected:
	pChunkBuffer cached_;	// hold materialized chunk

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual pBlock makeBlock(const blockId bId) = 0;
	virtual void makeBlocks(const bitmap blockBitmap);
	virtual void makeAllBlocks();
	virtual void insertBlock(pBlock inBlock) = 0;

	// mSize and mOffset size are not setted in the output of getBlockDesc function
	virtual pBlockDesc getBlockDesc(const blockId bId);
	size_t getBlockCapacity();
	virtual pBlock getBlock(const blockId bId) = 0;
	//virtual blockId getBlockId(pBlockDesc cDesc) = 0;
	//virtual blockId getBlockIdFromItemCoor(coor& itemCoor) = 0;
	//virtual blockId getBlockIdFromBlockCoor(coor& blockCoor) = 0;
	//virtual coor itemCoorToBlockCoor(coor& itemCoor) = 0;
	virtual coor getBlockCoor(const blockId bId);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) = 0;
	void copyBlockBitmap(cpBitmap blockBitmap);
	void replaceBlockBitmap(pBitmap blockBitmap);
	void mergeBlockBitmap(pBitmap blockBitmap);
	pBitmap getBlockBitmap();
	//cpBitmap getBlockBitmap() const;

protected:
	size_type blockCapacity_;
	pBitmap blockBitmap_;		// Be initialized to false by default

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pChunkItemIterator getItemIterator() = 0;
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;

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

//////////////////////////////
// Serializable
//////////////////////////////
protected:
	class chunkHeader : public serialHeader
	{
	public:
		static const char chunk_header_version = 1;

	public:
		virtual void serialize(std::ostream& os) override
		{
			//std::cout << "Chunk Header serialize" << std::endl;
			//std::cout << this->version_ << ", " << this->bodySize_ << std::endl;

			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->bodySize_), sizeof(this->bodySize_));
		}
		virtual void deserialize(std::istream& is) override
		{
			//std::cout << "Chunk Header deserialize" << std::endl;

			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->bodySize_), sizeof(this->bodySize_));

			//std::cout << this->version_ << ", " << this->bodySize_ << std::endl;
		}
	};

public:
	virtual void updateToHeader() override;
	virtual void updateFromHeader() override;
};
};
#endif	// _MSDB_CHUNK_H_