#pragma once
#ifndef _MSDB_MEMBLOCKITEMITERATOR_H_
#define _MSDB_MEMBLOCKITEMITERATOR_H_

#include <array/blockItemIterator.h>
#include <index/bitmap.h>

namespace msdb
{
class memBlockItemIterator : public blockItemIterator
{
public:
	using self_type = memBlockItemIterator;
	using base_type = blockItemIterator;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	// bSp : block start point
	memBlockItemIterator(void* data, 
						 const eleType eType, 
						 const dimension& dims,
						 const dimension& bSp,
						 pBitmap itemBitmap);
};

class memBlockItemRangeIterator : public blockItemRangeIterator
{
public:
	memBlockItemRangeIterator(void* data,
							  const eleType eType,
							  const dimension& dims,
							  const coorRange& range,
							  const dimension& bSp,
							  pBitmap itemBitmap);
};
}
#endif	// _MSDB_MEMBLOCKITEMITERATOR_H_

