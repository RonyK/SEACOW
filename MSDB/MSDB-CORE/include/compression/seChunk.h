#pragma once
#ifndef _MSDB_SE_CHUNK_H_
#define _MSDB_SE_CHUNK_H_

#include <array/blockChunk.h>
#include <array/block.h>
#include <vector>

namespace msdb
{
class seChunk;
using pSeChunk = std::shared_ptr<seChunk>;

class seChunk : public blockChunk
{
public:
	seChunk(pChunkDesc desc);
	virtual ~seChunk();

public:
	size_t getLevel();
	chunkId getBandId();
	//chunkId getTileId();
	chunkId getSourceChunkId();

	void setLevel(size_t level);
	void setBandId(chunkId bid);
	//void setTileId(chunkId tid);
	void setSourceChunkId(chunkId cid);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		auto it = this->getBlockIterator();
		blockId bid = 0;
		while (!it->isEnd())
		{
			// Set block bit
			size_t gap = this->rBitFromMMT[bid] - this->rBitFromDelta[bid];
			bs << setw(gap - 1) << (uint64_t)0;
			bs << setw(1) << 0x1;

			// Write value
			bs << setw(this->rBitFromDelta[bid]);
			auto bItemItr = (**it)->getItemIterator();
			while (!bItemItr->isEnd())
			{
				bs << (**bItemItr).get<Ty_>();
				++(*bItemItr);
			}

			++(*it);
			++bid;
		}
	}

	template<typename Ty_>
	void deserialize(bstream& bs)
	{
		auto it = this->getBlockIterator();
		blockId bid = 0;
		while (!it->isEnd())
		{
			//(*it)->deserialize(bs);
			bs >> setw(1);
			size_t gap = 0;
			char gapBit = 0;
			do
			{
				bs >> gapBit;
				++gap;
			} while (gapBit = 1);

			size_t bit = this->rBitFromMMT[bid] - gap;
			bs >> setw(bit);
			auto bItemItr = (**it)->getItemIterator();
			while(!bItemItr->isEnd())
			{
				Ty_ value;
				bs >> value;
				(**bItemItr).set<Ty_>(value);

				++(*bItemItr);
			}

			++(*it);
			++bid;
		}
	}
protected:
	size_t level_;
	chunkId bandId_;
	//chunkId tileId_;
	chunkId sourceChunkId_;

public:
	std::vector<bit_cnt_type> rBitFromMMT;		// required bits from MMT node
	std::vector<bit_cnt_type> rBitFromDelta;	// required bits from delta array
};
}	// msdb
#endif		// _MSDB_SE_CHUNK_H_