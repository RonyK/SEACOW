#pragma once
#ifndef _MSDB_CHUNKDESC_H_
#define _MSDB_CHUNKDESC_H_

namespace msdb
{
	class chunkDesc
	{
		using chunkId_type = unsigned int;

		chunkId_type _id;

	public:
		chunkDesc();

		chunkId_type getId();
	};
};

#endif