#pragma once
#ifndef _MSDB_MEMCHUNK_H_
#define _MSDB_MEMCHUNK_H_

#include <array/chunk.h>

namespace msdb
{
class memChunk : public chunk
{
public:
	memChunk(pChunkDesc desc);
	virtual ~memChunk();

protected:
	virtual void makeBuffer();

public:
	virtual chunkItemIterator getItemIterator();
	virtual chunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it.isEnd())
		{
			bs << (*it).get<Ty_>();
			++it;
		}
		this->serializedSize_ = bs.capacity();
	}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		bs >> setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it.isEnd())
		{
			Ty_ value;
			bs >> value;
			(*it).set<Ty_>(value);
			++it;
		}
	}
};
}
#endif	// _MSDB_MEMCHUNK_H_
