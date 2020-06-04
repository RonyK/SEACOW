#pragma once
#ifndef _MSDB_CHUNKDESC_H_
#define _MSDB_CHUNKDESC_H_

#include <array/chunkId.h>
#include <array/attributeId.h>

namespace msdb
{
class chunkDesc
{	
	struct chunkHeader
	{
		size_t bytes;

	};
public:
	chunkDesc();

	chunkId getId();

private:
	attributeId aid_;
	//coordinate coor_;
};
};

#endif