#pragma once
#ifndef _MSDB_HUFFMANBLOCK_H_
#define _MSDB_HUFFMANBLOCK_H_

#include <stdafx.h>
#include <array/memBlock.h>
#include <compression/adaptiveHuffmanCode.h>

namespace msdb
{
class huffmanBlock;
using pHuffmanBlock = std::shared_ptr<huffmanBlock>;

class huffmanBlock : public memBlock
{
public:
	huffmanBlock(pBlockDesc desc);
	virtual ~huffmanBlock();

public:
	template <typename Ty_>
	void serializeTy(aHuffmanCoder<Ty_>& coder, bstream& out)
	{
		auto iit = this->getItemIterator();

		while(!iit->isEnd())
		{
			coder.encode(out, (**iit).get<Ty_>());

			++(*iit);
		}
	}

	template <typename Ty_>
	void deserializeTy(aHuffmanCoder<Ty_>& coder, bstream& in)
	{
		auto iit = this->getItemIterator();

		while (!iit->isEnd())
		{
			Ty_ value = coder.decode(in);
			(**iit).set(value);

			++(*iit);
		}
	}
};
}		// msdb
#endif	// _MSDB_HUFFMANBLOCK_H_