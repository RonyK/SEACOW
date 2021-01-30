#pragma once
#ifndef _MSDB_ZIPCHUNK_H_
#define _MSDB_ZIPCHUNK_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <compression/compassBlock.h>
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
	void serializeTy(std::stringstream& ss)
	{
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		std::stringstream original;
		original.write((const char*)this->cached_->getReadData(), this->cached_->size());
		out.push(boost::iostreams::zlib_compressor());
		out.push(original);
		boost::iostreams::copy(out, ss);

		//////////////////////////////
		// TODO::Serialize block by block
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		//pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
		//		//cpBlock->serializeTy<Ty_>(ss);

		//		//blockId bId = (**blockItr)->getId();

		//		
		//		//original << (char*)this->cached_->getData() + (bId * mSizeBlock),
		//	}

		//	++(*blockItr);
		//}
	}

	template<class Ty_>
	void deserializeTy(std::stringstream& bs)
	{
		//////////////////////////////
		// TODO::deserialize zipChunk
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
		//		cpBlock->deserializeTy<Ty_>(bs);
		//	}

		//	++(*blockItr);
		//}
	}
};
}		// msdb
#endif	// _MSDB_ZIPCHUNK_H_
