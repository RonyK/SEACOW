#pragma once
#ifndef _MSDB_CHUNKCONTAINER_H_
#define _MSDB_CHUNKCONTAINER_H_

#include <array/chunk.h>
#include <map>

namespace msdb
{
using chunkContainer = std::map<chunkId, pChunk>;
using chunkPair = std::pair<chunkId, pChunk>;
}
#endif		// _MSDB_CHUNKCONTAINER_H_
