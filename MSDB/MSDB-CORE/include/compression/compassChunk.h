#pragma once
#ifndef _MSDB_COMPASSCHUNK_H_
#define _MSDB_COMPASSCHUNK_H_

#include <stdafx.h>
#include <array/memChunk.h>
#include <compression/compassBlock.h>
#include <io/bitstream.h>

namespace msdb
{
class compassChunk;
using pCompassChunk = std::shared_ptr<compassChunk>;

class compassChunk : public memChunk
{
public:
	compassChunk(pChunkDesc desc);
	virtual ~compassChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->serializeTy<Ty_>(bs);
			}

			++(*blockItr);
		}
	}

	template<class Ty_>
	void deserializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->deserializeTy<Ty_>(bs);
			}

			++(*blockItr);
		}
	}
};
}	// msdb
#endif	// _MSDB_COMPASSCHUNK_H_
