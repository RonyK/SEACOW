#pragma once
#ifndef _MSDB_COMPCHUNK_H_
#define _MSDB_COMPCHUNK_H_

#include <array/chunk.h>

namespace msdb
{
	class compChunk : public chunkBuffer
	{
	public:
		compChunk();

	public:
		bool isCompressed() const;
	};
};

#endif		// _MSDB_COMPCHUNK_H_