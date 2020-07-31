#pragma once
#ifndef _MSDB_MEMCHUNK_H_
#define _MSDB_MEMCHUNK_H_

#include <array/chunk.h>
#include <array/chunkIterator.h>

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
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it->isEnd())
		{
			bs << (**it).get<Ty_>();
			++(*it);
		}
	}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		bs >> setw(sizeof(Ty_) * CHAR_BIT);
		auto it = this->getItemIterator();

		while (!it->isEnd())
		{
			Ty_ value;
			bs >> value;
			(**it).set<Ty_>(value);
			++(*it);
		}
	}
};
}
#endif	// _MSDB_MEMCHUNK_H_
