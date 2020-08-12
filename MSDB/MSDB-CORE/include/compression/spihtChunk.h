#pragma once
#ifndef _MSDB_SPIHTCHUNK_H_
#define _MSDB_SPIHTCHUNK_H_

#include <array/chunk.h>
#include <io/bitstream.h>

namespace msdb
{
class spihtChunk : public chunk
{
public:
	spihtChunk(pChunkDesc desc);
	virtual ~spihtChunk();

protected:
	virtual void makeBuffer();

public:
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		// TODO::serialize
		//bs << setw(sizeof(Ty_) * CHAR_BIT);
		//auto it = this->getItemIterator();

		//while (!it->isEnd())
		//{
		//	bs << (**it).get<Ty_>();
		//	++(*it);
		//}
	}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		// TODO::deserialize
		//bs >> setw(sizeof(Ty_) * CHAR_BIT);
		//auto it = this->getItemIterator();

		//while (!it->isEnd())
		//{
		//	Ty_ value;
		//	bs >> value;
		//	(**it).set<Ty_>(value);
		//	++(*it);
		//}
	}
};

//class spihtChunkBuffer : public chunkBuffer
//{
//public:
//	spihtChunkBuffer();
//	virtual ~spihtChunkBuffer();
//
//public:
//	virtual void* getData();
//	virtual void const* getReadData() const;
//
//	virtual bufferSize size() const;
//
//	virtual void bufferAlloc(bufferSize size);
//	virtual void realloc(bufferSize size);
//	virtual void copy(void* data, bufferSize size);
//	virtual void copy(void* data, bufferSize offset, bufferSize size);
//	virtual void reference(void* data, bufferSize size);
//	virtual void free();
//
//protected:
//	bstream* data_;
//};
}
#endif	// _MSDB_SPIHTCHUNK_H_