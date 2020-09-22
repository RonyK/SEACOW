#pragma once
#ifndef _MSDB_COMPRESSION_H_
#define _MSDB_COMPRESSION_H_

#include <stdafx.h>

namespace msdb
{
enum class CompressionMethod
{
	NONE = 0,
	WAVELET,
	SPIHT,
	CA
};
}

#endif		// _MSDB_COMPRESSION_H_
