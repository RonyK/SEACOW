#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

namespace msdb
{
	class chunkBase
	{
	public:
		virtual bool isMaterialized() const;
		virtual bool isCompressed() const;
	};
};

#endif