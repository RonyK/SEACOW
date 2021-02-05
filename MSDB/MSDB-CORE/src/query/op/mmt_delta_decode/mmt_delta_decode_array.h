#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_

#include <stdafx.h>
#include <array/memBlockArray.h>

namespace msdb
{
class mmt_delta_decode_array : public memBlockArray
{
public:
	using base_type = memBlockArray;

public:
	mmt_delta_decode_array(pArrayDesc desc);
	~mmt_delta_decode_array();
};
}

#endif		// _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_