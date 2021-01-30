#pragma once
#ifndef _MSDB_ZIPBLOCK_H_
#define _MSDB_ZIPBLOCK_H_

#include <stdafx.h>
#include <array/memBlock.h>
#include <io/bitstream.h>

namespace msdb
{
class zipBlock;
using pZipBlock = std::shared_ptr<zipBlock>;

class zipBlock : public memBlock
{
public:
	zipBlock(pBlockDesc desc);
	virtual ~zipBlock();

public:
	template<typename Ty_>
	void serializeTy(bstream& bs)
	{

	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{

	}
};
}

#endif	// _MSDB_ZIPBLOCK_H_