#pragma once
#ifndef _MSDB_ADAPTHUFFMANBLOCK_H_
#define _MSDB_ADAPTHUFFMANBLOCK_H_

#include <stdafx.h>
#include <array/memBlock.h>
#include <compression/adaptiveHuffmanCode.h>

namespace msdb
{
class adaptHuffmanBlock;
using pAdaptHuffmanBlock = std::shared_ptr<adaptHuffmanBlock>;

class adaptHuffmanBlock : public memBlock
{
public:
	adaptHuffmanBlock(pBlockDesc desc);
	virtual ~adaptHuffmanBlock();

public:
	template<typename Cty_, typename Ty_>
	void serializeTy(aHuffmanCoder<Cty_, Ty_>& coder, bstream& out)
	{
		auto iit = this->getItemIterator();

		while (!iit->isEnd())
		{
			coder.encode(out, (**iit).get<Ty_>());

			++(*iit);
		}
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(aHuffmanCoder<Cty_, Ty_>& coder, bstream& in)
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
#endif	// _MSDB_ADAPTHUFFMANBLOCK_H_