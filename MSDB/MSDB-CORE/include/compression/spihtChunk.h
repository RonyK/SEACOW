#pragma once
#ifndef _MSDB_SPIHTCHUNK_H_
#define _MSDB_SPIHTCHUNK_H_

#include <array/memChunk.h>
#include <compression/spihtBlock.h>
#include <io/bitstream.h>
#include <list>

namespace msdb
{
class spihtChunk;
using pSpihtChunk = std::shared_ptr<spihtChunk>;

class spihtChunk : public memChunk
{
public:
	spihtChunk(pChunkDesc desc);
	virtual ~spihtChunk();

protected:
	virtual void makeBlocks(std::vector<bool> bitmap);
	virtual void makeBuffer();

public:
	//virtual pChunkItemIterator getItemIterator();
	//virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			this->blockSerialize<Ty_>(bs, (**blockItr));
			++(*blockItr);
		}
		// TODO::serialize
		//bs << setw(sizeof(Ty_) * CHAR_BIT);
		//auto it = this->getItemIterator();

		//while (!it->isEnd())
		//{
		//	bs << (**it).get<Ty_>();
		//	++(*it);
		//}
	}

	template <typename Ty_>
	void blockSerialize(bstream& bs, pBlock curBlock)
	{
		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
		spBlock->serializeTy<Ty_>(bs);
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
}
#endif	// _MSDB_SPIHTCHUNK_H_