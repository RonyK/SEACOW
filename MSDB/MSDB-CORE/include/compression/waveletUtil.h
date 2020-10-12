#pragma once
#ifndef _MSDB_WAVELETUTIL_H_
#define _MSDB_WAVELETUTIL_H_

#include <stdafx.h>
#include <util/coordinate.h>

namespace msdb
{
coorRange getBandRange(const size_t bandId, const coor& bandDims);
}		// msdb
#endif	// _MSDB_WAVELETUTIL_H_