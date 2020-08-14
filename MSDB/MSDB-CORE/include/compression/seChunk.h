#pragma once
#ifndef _MSDB_SE_CHUNK_H_
#define _MSDB_SE_CHUNK_H_

#include <array/blockChunk.h>
#include <array/block.h>
#include <compression/waveletUtil.h>
#include <vector>

namespace msdb
{
class seChunk;
using pSeChunk = std::shared_ptr<seChunk>;

class seChunk : public memBlockChunk
{
public:
	seChunk(pChunkDesc desc);
	virtual ~seChunk();

public:
	size_t getLevel();
	//chunkId getSourceChunkId();

	void setLevel(size_t level);
	//void setSourceChunkId(chunkId cid);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	void serializeGap(bstream& bs, size_t gap);
	bit_cnt_type deserializeGap(bstream& bs);

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t bandSeqId = 0;
		// Band 0
		{
			this->serializeBand<Ty_>(bs, myBlock, bandSeqId, 0, bandDims);
			++bandSeqId;
		}

		for (size_t level = 0; level <= this->level_; ++level)
		{
			for (size_t band = 1; band <= numBandsInLevel; ++band, ++bandSeqId)
			{
				this->serializeBand<Ty_>(bs, myBlock, bandSeqId, band, bandDims);
			}

			bandDims *= 2;
		}
	}

	template<typename Ty_>
	void serializeBand(bstream& bs, pBlock myBlock,
					   size_t bandSeqId, size_t bandId, dimension bandDims)
	{
		bit_cnt_type requiredBits = this->rBitFromDelta[bandSeqId];
		this->serializeGap(bs, this->rBitFromMMT - requiredBits);

		bs << setw(requiredBits);
		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			bs << (**bItemItr).get<Ty_>();
			++(*bItemItr);
		}
	}

	template <typename Ty_>
	void deserializeBand(bstream& bs, pBlock myBlock,
						 size_t bandSeqId, size_t bandId, dimension bandDims)
	{
		size_t gap = this->deserializeGap(bs);
		size_t requiredBit = this->rBitFromMMT - gap;

		bs >> setw(requiredBit);
		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			Ty_ value;
			bs >> value;
			(**bItemItr).set<Ty_>(value);
			++(*bItemItr);
		}

		//while (!bItemItr->isEnd())
		//{
		//	Ty_ value;
		//	bs >> value;
		//	(**bItemItr).set<Ty_>(value);

		//	++(*bItemItr);
		//}
	}

	template<typename Ty_>
	void deserialize(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t bandSeqId = 0;
		// Band 0
		{
			this->deserializeBand<Ty_>(bs, myBlock, bandSeqId, 0, bandDims);
		}

		for (size_t level = 0; level <= this->level_; ++level)
		{
			for (size_t band = 1; band <= numBandsInLevel; ++band, ++bandSeqId)
			{
				this->deserializeBand<Ty_>(bs, myBlock, bandSeqId, band, bandDims);
			}

			bandDims *= 2;
		}

		//size_t gap = this->deserializeGap(bs);

		//size_t bit = this->rBitFromMMT - gap;
		//bs >> setw(bit);
		//auto bItemItr = myBlock->getItemIterator();
		//while (!bItemItr->isEnd())
		//{
		//	Ty_ value;
		//	bs >> value;
		//	(**bItemItr).set<Ty_>(value);

		//	++(*bItemItr);
		//}

		//auto it = this->getBlockIterator();
		//blockId bid = 0;
		//while (!it->isEnd())
		//{
		//	//(*bit)->deserialize(bs);
		//	bs >> setw(1);
		//	size_t gap = 0;
		//	char gapBit = 0;
		//	do
		//	{
		//		bs >> gapBit;
		//		++gap;
		//	} while (gapBit = 1);

		//	size_t bit = this->rBitFromMMT[bid] - gap;
		//	bs >> setw(bit);
		//	auto bItemItr = (**it)->getItemIterator();
		//	while(!bItemItr->isEnd())
		//	{
		//		Ty_ value;
		//		bs >> value;
		//		(**bItemItr).set<Ty_>(value);

		//		++(*bItemItr);
		//	}

		//	++(*it);
		//	++bid;
		//}
	}
protected:
	size_t level_;
	//chunkId sourceChunkId_;

public:
	bit_cnt_type rBitFromMMT;	// required bits from MMT node
	std::vector<bit_cnt_type> rBitFromDelta;	// required bits from delta array
};
}	// msdb
#endif		// _MSDB_SE_CHUNK_H_