#include <array/memBlockItemIterator.h>

namespace msdb
{
memBlockItemIterator::memBlockItemIterator(void* data,
                                           const eleType eType, 
                                           const dimension& dims,
                                           const dimension& bSp)
    : blockItemIterator(data, eType, dims, bSp),
    coorItr(dims.size(), dims.data())
{
}
memBlockItemRangeIterator::memBlockItemRangeIterator(void* data,
                                                     const eleType eType,
                                                     const dimension& dims, 
                                                     const coorRange& range, 
                                                     const dimension& bSp)
    : blockItemRangeIterator(data, eType, dims, range, bSp),
    coorItr(dims.size(), dims.data())
{
}
}	// msdb