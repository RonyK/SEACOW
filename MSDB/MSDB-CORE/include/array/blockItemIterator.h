#pragma once
#ifndef _MSDB_BLOCKITEM_ITERATOR_H_
#define _MSDB_BLOCKITEM_ITERATOR_H_

#include <util/coordinate.h>
#include <array/dimension.h>

namespace msdb
{
class blockItemIterator;
class blockItemRangeIterator;
using pBlockItemIterator = std::shared_ptr<blockItemIterator>;
using pBlockItemIterator = std::shared_ptr<blockItemIterator>;
using pBlockItemRangeIterator = std::shared_ptr<blockItemRangeIterator>;
using pBlockItemRangeIterator = std::shared_ptr<blockItemRangeIterator>;

template <typename Dty_>
class blockItemIteratorBase : virtual public coordinateIterator<Dty_>
{
public:
	using base_type = coordinateIterator<Dty_>;
	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	blockItemIteratorBase(void* data, 
						  const eleType eType, 
						  const dimension dims,
						  const dimension bSp)
		: base_type(dims), bSp_(bSp)
	{
	}

public:
	//coordinate_type coorOut2In(coordinate_type& out)
	//{

	//}

	//coordinate_type coorIn2Out(coordinate_type& in)
	//{
	//}

	//coordinate_type coorIn2Out()
	//{
	//}

	//coordinate_type ceP()
	//{
	//}

	//coordinate_type outCoor()
	//{
	//}

protected:
	coordinate_type bSp_;
};

using blockItemItrBase = blockItemIteratorBase<position_t>;

class blockItemIterator : public itemItr, public blockItemItrBase
{
public:
	using self_type = blockItemIterator;
	using base_type = itemItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	blockItemIterator(void* data, const eleType eType, 
					  const dimension dims, const dimension bSp);
};

class blockItemRangeIterator : public itemRangeItr, public blockItemItrBase
{
public:
	using base_type = itemRangeItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;


public:
	blockItemRangeIterator(void* data, const eleType eType,
						   const dimension dims,
						   const coorRange& range,
						   const dimension bSp);
};

}	// msdb
#endif	// _MSDB_BLOCKITEM_ITERATOR_H_
