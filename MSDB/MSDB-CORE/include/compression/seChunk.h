#pragma once
#ifndef _MSDB_SE_CHUNK_H_
#define _MSDB_SE_CHUNK_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <array/block.h>
#include <compression/waveletUtil.h>

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
		//std::cout << "Serialize Chunk" << std::endl;
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
		//std::cout << "==SerializeBand==" << std::endl;
		bit_cnt_type requiredBits = this->rBitFromDelta[bandSeqId];
		//std::cout << "rBit: " << static_cast<int>(requiredBits) << " / gap : " << static_cast<int>(this->rBitFromMMT - requiredBits) << std::endl;;
		//bs.print();
		assert(this->rBitFromMMT >= requiredBits);
		//std::cout << static_cast<int>(this->rBitFromMMT - requiredBits) << " / " << static_cast<int>(this->rBitFromMMT) << std::endl;
		this->serializeGap(bs, this->rBitFromMMT - requiredBits);
		//bs.print();

		bs << setw(requiredBits);
		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			Ty_ value = (**bItemItr).get<Ty_>();
			bs << (**bItemItr).get<Ty_>();
			++(*bItemItr);
			//std::cout << "Value: " << static_cast<int>(value) << std::endl;
			//bs.print();
		}
	}

	template <typename Ty_>
	void deserializeBand(bstream& bs, pBlock myBlock,
						 const size_t bandSeqId, const size_t bandId, dimension& bandDims)
	{
		size_t gap = this->deserializeGap(bs);
		size_t requiredBit = this->rBitFromMMT - gap;
		//std::cout << static_cast<int>(gap) << " / " << static_cast<int>(this->rBitFromMMT) << std::endl;
		assert(gap <= this->rBitFromMMT);

		bs >> setw(requiredBit);
		Ty_ signMask = 0x1 << requiredBit - 1;
		Ty_ negativeMask = (Ty_)-1 - (signMask - 1);
		
		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			Ty_ value = 0;
			bs >> value;
			if(value & signMask)
			{
				value |= negativeMask;
			}

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

template <typename mmtNode>
bit_cnt_type getRBitFromMMT(mmtNode node)
{
	if (node->order_ > 1)
	{
		if (SIGN(node->bMax_) > 0)
		{
			return node->bMax_;
		}

		return abs_(node->bMin_);
	}

	if (SIGN(node->bMax_) != SIGN(node->bMin_))
	{
		// bMax > 0
		//return std::max((bit_cnt_type)node->bMax_, (bit_cnt_type)abs_(node->bMin_)) + 1;	// 여기 왜 1을 추가한거지?
		return std::max((bit_cnt_type)node->bMax_, (bit_cnt_type)abs_(node->bMin_));
	}

	return std::max((bit_cnt_type)abs_(node->bMax_), (bit_cnt_type)abs_(node->bMin_));
}
}	// msdb
#endif		// _MSDB_SE_CHUNK_H_