#pragma once
#ifndef _MSDB_LZWBLOCK_H_
#define _MSDB_LZWBLOCK_H_

#include <stdafx.h>
#include <array/memBlock.h>
#include <util/ioutil.h>

namespace msdb
{
class lzwBlock;
using pLzwBlock = std::shared_ptr<lzwBlock>;

class lzwBlock : public memBlock
{
public:
	lzwBlock(pBlockDesc desc);
	virtual ~lzwBlock();

public:
	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
        
	}

	template<typename Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		
	}
};
}

#endif	// _MSDB_LZWBLOCK_H_