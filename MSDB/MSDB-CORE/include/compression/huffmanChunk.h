#pragma once
#ifndef _MSDB_HUFFMANCHUNK_H_
#define _MSDB_HUFFMANCHUNK_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <compression/huffmanBlock.h>

namespace msdb
{
class huffmanChunk;
using pHuffmanChunk = std::shared_ptr<huffmanChunk>;

class huffmanChunk : public memBlockChunk
{
public:
	huffmanChunk(pChunkDesc desc);
	virtual ~huffmanChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

private:
	template<typename Cty_, typename Ty_>
	void serializeTy(bstream& out)
	{
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);

		//auto bit = this->getBlockIterator();
		//while(!bit->isEnd())
		//{
		//	if(bit->isExist())
		//	{
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->serializeTy<Cty_, Ty_>(coder, out);
		//	}

		//	++(*bit);
		//}

		aHuffmanCoder<uint32_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (const char*)this->getBuffer()->getReadData();
		auto size = this->getBuffer()->size();
		for(int i = 0; i < size; ++i)
		{
			coder.encode(out, buffer[i]);
		}
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(bstream& in)
	{
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);

		//auto bit = this->getBlockIterator();
		//while (!bit->isEnd())
		//{
		//	if (bit->isExist())
		//	{
		//		auto a = *bit;
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->deserializeTy<Cty_, Ty_>(coder, in);
		//	}

		//	++(*bit);
		//}

		aHuffmanCoder<uint16_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (unsigned char*)this->getBuffer()->getData();
		auto size = this->getBuffer()->size();
		for(int i = 0; i < size; ++i)
		{
			unsigned char value = coder.decode(in);
			buffer[i] = value;
		}
	}
};
}		// msdb
#endif	// _MSDB_HUFFMANCHUNK_H_