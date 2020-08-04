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
	pChunkBuffer cached_;	// hold materialized chunk

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	void free();
	// NOTE::Call a referenceBufferToBlock() in a makeBuffer()
	virtual void makeBuffer() = 0;
	virtual void referenceBufferToBlock() = 0;

public:
	virtual void alloc();
	virtual void alloc(bufferSize size);
	virtual void materializeCopy(void* data, bufferSize size);
	virtual void materializeAssign(void* data, bufferSize size);
	//virtual void materializeCopy(bstream& bs);
	bool isMaterialized() const;

protected:
	pChunkDesc desc_;		// chunk desc

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual void makeBlocks(std::vector<bool> bitmap) = 0;
	virtual void makeAllBlocks();
	virtual size_t getBlockCapacity() = 0;
	virtual pBlock getBlock(blockId bId) = 0;
	virtual blockId getBlockId(pBlockDesc cDesc) = 0;
	virtual blockId getBlockIdFromItemCoor(coor& itemCoor) = 0;
	virtual blockId getBlockIdFromBlockCoor(coor& blockCoor) = 0;
	virtual virtual coor itemCoorToBlockCoor(coor& itemCoor) = 0;
	virtual pBlockIterator getBlockIterator(iterateMode itMode = iterateMode::ALL) = 0;

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
			std::cout << "Chunk Header serialize" << std::endl;
			std::cout << this->version_ << ", " << this->bodySize_ << std::endl;

			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->bodySize_), sizeof(this->bodySize_));
		}
		virtual void deserialize(std::istream& is) override
		{
			std::cout << "Chunk Header deserialize" << std::endl;

			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->bodySize_), sizeof(this->bodySize_));

			std::cout << this->version_ << ", " << this->bodySize_ << std::endl;
		}
	};

public:
	virtual void updateToHeader() override;
	virtual void updateFromHeader() override;
};
};
#endif	// _MSDB_CHUNK_H_