#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_

#include <array/array.h>

namespace msdb
{
class mmt_delta_decode_array : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	mmt_delta_decode_array(pArrayDesc desc);
	~mmt_delta_decode_array();
};
}

#endif		// _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_