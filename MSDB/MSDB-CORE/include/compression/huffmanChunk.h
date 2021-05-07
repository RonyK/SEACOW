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
	template<typename Ty_>
	void serializeTy(bstream& out)
	{
		aHuffmanCoder<Ty_> coder(sizeof(Ty_) * CHAR_BIT);

		auto bit = this->getBlockIterator();
		while(!bit->isEnd())
		{
			if(bit->isExist())
			{
				pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
				hb->serializeTy<Ty_>(coder, out);
			}

			++(*bit);
		}
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		aHuffmanCoder<Ty_> coder(sizeof(Ty_) * CHAR_BIT);

		auto bit = this->getBlockIterator();
		while (!bit->isEnd())
		{
			if (bit->isExist())
			{
				auto a = *bit;
				pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
				hb->deserializeTy<Ty_>(coder, in);
			}

			++(*bit);
		}
	}
};
}		// msdb
#endif	// _MSDB_HUFFMANCHUNK_H_