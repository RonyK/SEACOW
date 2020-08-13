#pragma once
#ifndef _MSDB_WAVELETUTIL_H_
#define _MSDB_WAVELETUTIL_H_

#include <util/coordinate.h>

namespace msdb
{
coorRange getBandRange(size_t bandId, coor& bandDims);
}		// msdb
#endif	// _MSDB_WAVELETUTIL_H_