#pragma once
#ifndef _MSDB_MEMARRAY_H_
#define _MSDB_MEMARRAY_H_

#include <array/array.h>

namespace msdb
{
class memArray : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	memArray(pArrayDesc desc);
	~memArray();
};
}	// msdb
#endif	// _MSDB_MEMARRAY_H_
