#pragma once
#ifndef _MSDB_WAVELETUTIL_H_
#define _MSDB_WAVELETUTIL_H_

#include <stdafx.h>
#include <util/coordinate.h>

namespace msdb
{
coorRange getBandRange(const size_t bandId, const coor& bandDims);

// Get wavelet band info

struct blockInfo
{
	size_t curLevel;
	size_t curBand;
};

using pVecBlockInfo = std::shared_ptr<std::vector<blockInfo>>;

pVecBlockInfo getWaveletBlockInfo(size_t maxLevel, size_t dSize, size_t blockId);

}		// msdb
#endif	// _MSDB_WAVELETUTIL_H_