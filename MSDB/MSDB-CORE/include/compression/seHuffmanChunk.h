#pragma once
#ifndef _MSDB_SEHUFFMANCHUNK_H_
#define _MSDB_SEHUFFMANCHUNK_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <io/bitstream.h>
#include <compression/seChunk.h>
#include <compression/huffmanCode.h>
#include <compression/fixedHuffmanCoder.h>

namespace msdb
{
namespace core
{
class seHuffmanChunk;
using pSeHuffmanChunk = std::shared_ptr<seHuffmanChunk>;

class seHuffmanChunk : public seChunk
{
public:
	seHuffmanChunk(pChunkDesc desc);
	virtual ~seHuffmanChunk();

	using lzwCodeType = std::uint16_t;

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		this->seEncode<Ty_>(bs);
	}

	template <typename Ty_>
	void seEncode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);
		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);
		size_t seqId = 0;
		// Level 0
		{
			for (size_t band = 0; band <= numBandsInLevel; ++band, ++seqId)
			{
				this->serializeBand<Ty_>(bs, myBlock, seqId, band, bandDims);
			}
		}

		this->serializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	template<typename Ty_>
	void serializeBand(bstream& bs, pBlock myBlock,
					   size_t seqId, size_t bandId, dimension& bandDims)
	{
		bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

		this->serializeGap(bs, rbFromMMT - rbFromDelta);
		bs << setw(rbFromDelta);

		Ty_ signMask = 0x1 << rbFromDelta - 1;
		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		//assert(rbFromDelta <= 16);
		auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];
		size_t beforeBitPos = bs.getOutBitPos();

		while (!bItemItr->isEnd())
		{
			Ty_ value = (**bItemItr).get<Ty_>();

			if (value < 0)
			{
				value = abs_(value);
				value |= signMask;
			}

			coder->encode(bs, &value, 1);
			//bs << value;
 			++(*bItemItr);
		}

		auto after = bs.getOutBitPos();
		//BOOST_LOG_TRIVIAL(debug) << "Bits: " << bs.getOutBitPos() - beforeBitPos;
	}

	template <typename Ty_>
	void serializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		//BOOST_LOG_TRIVIAL(debug) << "SE HUFFMAN CHUNK - serializeChildLevelBand";
		auto dSize = this->getDSize();

		for (size_t level = 1; level <= this->level_; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			coorItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + this->getChunkCoor() * innerSpace);
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
					dimension targetEp = targetSp + bandDims;
					auto bItemItr = inBlock->getItemRangeIterator(coorRange(targetSp, targetEp));

					bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

					//BOOST_LOG_TRIVIAL(debug) << "Lv: " << level << ", Bd: " << band << ", Block: " << bs.getOutBlockPos() << ", Bit: " << bs.getOutBitPosInBlock();
					//BOOST_LOG_TRIVIAL(debug) << "Block: " << bs.getOutLastBlock();

					this->serializeGap(bs, (int64_t)rbFromMMT - (int64_t)rbFromDelta);

					//BOOST_LOG_TRIVIAL(debug) << "Gap: " << (int64_t)rbFromMMT - (int64_t)rbFromDelta << ", After Block: " << bs.getOutLastBlock();
					//BOOST_LOG_TRIVIAL(debug) << "delta: " << static_cast<int64_t>(rbFromDelta) << " / mmt: " << static_cast<int64_t>(rbFromMMT) << " / gap: " << (int64_t)rbFromMMT - (int64_t)rbFromDelta;

					//assert(rbFromDelta <= 8);
					//iFixedHuffmanCoder coder[8] = { nullptr };
					//for (size_t bi = 0; bi < (size_t)(rbFromDelta + CHAR_BIT - 1) / (size_t)CHAR_BIT; ++bi)
					//{
					//	coder[bi] = this->fixedHuffmanCoders[std::min((bit_cnt_type)rbFromDelta - bi * CHAR_BIT, (bit_cnt_type)CHAR_BIT)];
					//}
					auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

					if (rbFromDelta != 0)
					{
						size_t beforeBitPos = bs.getOutBitPos();
						bs << setw(rbFromDelta);
						Ty_ signMask = 0x1 << rbFromDelta - 1;

						while (!bItemItr->isEnd())
						{
							Ty_ value = (**bItemItr).get<Ty_>();
							if (value < 0)
							{
								value = abs_(value);
								value |= signMask;
							}
							coder->encode(bs, &value, 1);
							//bs << value;
							++(*bItemItr);
						}

						//BOOST_LOG_TRIVIAL(debug) << "Bits: " << bs.getOutBitPos() - beforeBitPos;
					}

					++seqId;
				}
				++innerItr;
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		this->seDecode<Ty_>(bs);
	}

	template <typename Ty_>
	void seDecode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t seqId = 0;
		// Level 0
		{
			for (size_t band = 0; band <= numBandsInLevel; ++band, ++seqId)
			{
				this->deserializeBand<Ty_>(bs, myBlock, seqId, band, bandDims);
			}
		}

		this->deserializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	template <typename Ty_>
	void deserializeBand(bstream& bs, pBlock myBlock,
						 const size_t seqId, const size_t bandId, dimension& bandDims)
	{
		int64_t gap = this->deserializeGap(bs);
		assert(gap <= 64);
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
		size_t rbFromDelta = rbFromMMT - gap;

		bs >> setw(rbFromDelta);
		Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
		Ty_ negativeMask = (Ty_)-1 ^ signMask;
		Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		//assert(rbFromDelta <= 16);
		auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

		//////////////////////////////
		// 01
		auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
		auto itemCapa = bandDims.area();
		char* spData = (char*)this->getBuffer()->getData() + spOffset;

		size_t tempBufferSize = std::min({ (size_type)(rbFromDelta * itemCapa * sizeof(Ty_) / CHAR_BIT * 2), (size_type)(bs.capacity() - bs.getInBlockPos()) });
		bstream bsHuffman;
		bsHuffman.resize(tempBufferSize);
		memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
		bsHuffman.jumpBits(bs.getInBitPosInBlock());

		Ty_* huffmanDecoded = new Ty_[itemCapa];
		auto readBits = coder->decode(static_cast<void*>(huffmanDecoded), itemCapa, bsHuffman);
		bs.jumpBits(readBits);
		//BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;

		if ((Ty_)-1 < 0)
		{
			// Ty_ has negative values
			for (int i = 0; i < itemCapa; ++i)
			{
				auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
				*pValue = huffmanDecoded[i];
				//bs >> *pValue;
				
				if (*pValue & signMask)
				{
					*pValue &= negativeMask;
					*pValue *= -1;
					*pValue |= signBit;
				}
			}
		}
		else
		{
			// Ty_ only has positive values
			for (int i = 0; i < itemCapa; ++i)
			{
				auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
				*pValue = huffmanDecoded[i];
			}
		}

		delete[] huffmanDecoded;
		//////////////////////////////
	}

	template <typename Ty_>
	void deserializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		//BOOST_LOG_TRIVIAL(debug) << "SE HUFFMAN CHUNK - deserializeChildLevelBand";
		auto dSize = this->getDSize();
		for (size_t level = 1; level <= this->level_; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			coorItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + this->getChunkCoor() * innerSpace);
				//BOOST_LOG_TRIVIAL(trace) << "area: " << static_cast<int>(innerCoor.area());
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
					dimension targetEp = targetSp + bandDims;

					//BOOST_LOG_TRIVIAL(debug) << "Lv: " << level << ", Bd: " << band << ", Block: " << bs.getInBlockPos() << ", Bit: " << bs.getInBitPosInBlock();
					//BOOST_LOG_TRIVIAL(debug) << "Block: " << bs.getInFrontBlock();
					
					int64_t gap = this->deserializeGap(bs);
					
					//BOOST_LOG_TRIVIAL(debug) << "Gap: " << (int64_t)gap << ", After Block: " << bs.getInFrontBlock();

					if (gap > 64)
					{
						assert(gap <= 64);
					}
					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
					size_t rbFromDelta = rbFromMMT - gap;
					
					//BOOST_LOG_TRIVIAL(debug) << "delta: " << static_cast<int64_t>(rbFromDelta) << " / mmt: " << static_cast<int64_t>(rbFromMMT) << " / gap: " << static_cast<int64_t>(gap);

					auto bItemItr = inBlock->getItemRangeIterator(coorRange(targetSp, targetEp));
					//////////////////////////////
					// 01
					auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
					auto itemCapa = bandDims.area();
					char* spData = (char*)this->getBuffer()->getData() + spOffset;
					//////////////////////////////

					if (rbFromDelta == 0)
					{
						//////////////////////////////
						// 01
						Ty_ value = 0;
						for (size_type i = 0; i < itemCapa; ++i)
						{
							*((Ty_*)(spData + this->tileOffset_[i])) = 0;
						}
						//////////////////////////////
					}
					else
					{
						//assert(rbFromDelta <= 16);
						auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

						// Read more buffer
						size_t tempBufferSize = std::min({ (size_type)(rbFromDelta * itemCapa * sizeof(Ty_) / (double)CHAR_BIT * 2), (size_type)(bs.capacity() - bs.getInBlockPos()) });
						bstream bsHuffman;
						bsHuffman.resize(tempBufferSize);
						memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
						bsHuffman.jumpBits(bs.getInBitPosInBlock());

						Ty_* huffmanDecoded = new Ty_[itemCapa];
						auto readBits = coder->decode(static_cast<void*>(huffmanDecoded), itemCapa, bsHuffman);
						//BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;
						bs.jumpBits(readBits);

						bs >> setw(rbFromDelta);
						Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
						Ty_ negativeMask = (Ty_)-1 ^ signMask;
						Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

						//////////////////////////////
						// 01
						if ((Ty_)-1 < 0)
						{
							for (size_type i = 0; i < itemCapa; ++i)
							{
								auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
								*pValue = huffmanDecoded[i];

								if (*pValue & signMask)
								{
									*pValue &= negativeMask;
									*pValue *= -1;
									*pValue |= signBit;
								}
							}
						}
						else
						{
							for (size_type i = 0; i < itemCapa; ++i)
							{
								auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
								*pValue = huffmanDecoded[i];
							}
						}
						//////////////////////////////

						delete[] huffmanDecoded;
					}
					++seqId;
				}
				++innerItr;
			}
		}
	}

	//////////////////////////////
	//template<typename Ty_>
	//void serializeTy(bstream& out)
	//{
	//	bstream seOut;
	//	this->seEncode<Ty_>(seOut);
	//	
	//	out << setw(sizeof(size_type) * CHAR_BIT) << seOut.capacity();

	//	//////////////////////////////
	//	huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
	//	huffmanEncoder.encode(out, (uint8_t*)seOut.data(), seOut.capacity());
	//}

	//template<class Ty_>
	//void deserializeTy(bstream& in)
	//{
	//	size_type seSize = 0;
	//	in >> setw(sizeof(size_type) * CHAR_BIT) >> seSize;
	//	uint8_t* tempBuffer = new uint8_t[seSize];

	//	huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
	//	huffmanDecoder.decode((uint8_t*)tempBuffer, seSize, in);

	//	//////////////////////////////
	//	bstream seIn;
	//	seIn.resize(seSize);
	//	memcpy(seIn.data(), tempBuffer, seSize);
	//	delete[] tempBuffer;

	//	this->seDecode<Ty_>(seIn);
	//}

private:
	static std::vector<std::vector<iFixedHuffmanCoder*>> fixedHuffmanCoders;
};
}		// core
}		// msdb
#endif	// _MSDB_SEHUFFMANCHUNK_H_