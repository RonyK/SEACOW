#pragma once
#ifndef _MSDB_ZIPCHUNK_H_
#define _MSDB_ZIPCHUNK_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <compression/zipBlock.h>
#include <io/bitstream.h>

namespace msdb
{
class zipChunk;
using pZipChunk = std::shared_ptr<zipChunk>;

class zipChunk : public memBlockChunk
{
public:
	zipChunk(pChunkDesc desc);
	virtual ~zipChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
				zBlock->serializeTy<Ty_>(compressed);
			}

			++(*blockItr);
		}
	}

	template<class Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		this->bufferAlloc();

		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
				zBlock->deserializeTy<Ty_>(compressed);
			}

			++(*blockItr);
		}
	}
};
}		// msdb
#endif	// _MSDB_ZIPCHUNK_H_
