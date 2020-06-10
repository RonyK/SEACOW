#pragma once
#ifndef _MSDB_COMPCHUNK_H_
#define _MSDB_COMPCHUNK_H_

#include <array/chunk.h>
#include <array/chunkBuffer.h>

namespace msdb
{
	class compChunk : public chunkBuffer
	{
	public:
		bool isCompressed() const;
	};
};

#endif