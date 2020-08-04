#include <array/blockItemIterator.h>

namespace msdb
{
blockItemIterator::blockItemIterator(void* data, 
									 const eleType eType,
									 const dimension dims,
									 const dimension bSp)
	: itemItr(data, eType, dims), 
	blockItemIteratorBase(data, eType, dims, bSp), 
	coorItr(dims)
{
}
blockItemRangeIterator::blockItemRangeIterator(void* data,
											   const eleType eType,
											   const dimension dims, 
											   const coorRange& range,
											   const dimension bSp)
	: itemRangeItr(data, eType, dims, range),
	blockItemIteratorBase(data, eType, dims, bSp),
	coorItr(dims)
{
}
}