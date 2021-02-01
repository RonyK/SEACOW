#pragma once
#ifndef _MSDB_BLOCKCONTAINER_H_
#define _MSDB_BLOCKCONTAINER_H_

#include <stdafx.h>
#include <array/block.h>

namespace msdb
{
//using blockContainer = std::map<blockId, pBlock>;
using blockContainer = std::vector<pBlock>;
using blockPair = std::pair<blockId, pBlock>;
}
#endif // _MSDB_BLOCKCONTAINER_H_
