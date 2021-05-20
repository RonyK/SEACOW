#pragma once
#ifndef _MSDB_LZWHUFFMANCHUNK_H_
#define _MSDB_LZWHUFFMANCHUNK_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <io/bitstream.h>
#include <compression/huffmanCode.h>

namespace msdb
{
class seHuffmanChunk;
using pSeHuffmanChunk = std::shared_ptr<seHuffmanChunk>;

class seHuffmanChunk : public memBlockChunk
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
	void serializeTy(bstream& out)
	{
		//bstream lzwOut;
		//lzwCoder<lzwCodeType> lzwEncoder;
		//lzwEncoder.encode(lzwOut, (const unsigned char*)this->cached_->getReadData(), this->cached_->size());

		//out << setw(sizeof(size_type) * CHAR_BIT) << lzwOut.capacity();

		////////////////////////////////
		//huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		//huffmanEncoder.encode(out, (uint8_t*)lzwOut.data(), lzwOut.capacity());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		//size_type lzwSize = 0;
		//in >> setw(sizeof(size_type) * CHAR_BIT) >> lzwSize;

		//uint8_t* tempBuffer = new uint8_t[lzwSize];

		//huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		//huffmanDecoder.decode((uint8_t*)tempBuffer, lzwSize, in);

		//bstream lzwIn;
		//lzwIn.resize(lzwSize);
		//memcpy(lzwIn.data(), tempBuffer, lzwSize);

		//lzwCoder<lzwCodeType> lzwDecoder;
		//lzwDecoder.decode((unsigned char*)this->cached_->getData(), this->cached_->size(), in);
	}
};
}		// msdb
#endif	// _MSDB_LZWHUFFMANCHUNK_H_