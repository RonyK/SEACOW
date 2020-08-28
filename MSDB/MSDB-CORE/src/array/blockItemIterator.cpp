#include <array/blockItemIterator.h>

namespace msdb
{
blockItemIterator::blockItemIterator(void* data, 
									 const eleType eType,
									 const dimension& dims,
									 const dimension& bSp,
									 pBitmap itemBitmap)
	: itemItr(data, eType, dims), 
	blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	coorItr(dims)
{
}
blockItemRangeIterator::blockItemRangeIterator(void* data,
											   const eleType eType,
											   const dimension& dims, 
											   const coorRange& range,
											   const dimension& bSp,
											   pBitmap itemBitmap)
	: itemRangeItr(data, eType, dims, range),
	blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	coorItr(dims)
{
}
}