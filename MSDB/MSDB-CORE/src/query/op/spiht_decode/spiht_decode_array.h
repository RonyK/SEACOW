#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_ARRAY_H_
#define _MSDB_OP_SPIHT_DECODE_ARRAY_H_

#include <array/array.h>

namespace msdb
{
class spiht_decode_array : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	spiht_decode_array(pArrayDesc desc, const size_t maxLevel);

public:
	size_t getMaxLevel();

protected:
	size_t maxLevel_;

};
}


#endif // _MSDB_OP_SPIHT_DECODE_ARRAY_H_