#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
#define _MSDB_OP_WAVELET_ENCODE_ARRAY_H_

#include <array/array.h>

namespace msdb
{
class wavelet_encode_array : public arrayBase
{
public:
	wavelet_encode_array(pArrayDesc desc);
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
