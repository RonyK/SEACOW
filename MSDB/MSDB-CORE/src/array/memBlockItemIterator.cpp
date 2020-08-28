#include <array/memBlockItemIterator.h>

namespace msdb
{
memBlockItemIterator::memBlockItemIterator(void* data,
                                           const eleType eType, 
                                           const dimension& dims,
                                           const dimension& bSp,
                                           pBitmap itemBitmap)
    : blockItemIterator(data, eType, dims, bSp, itemBitmap),
    coorItr(dims.size(), dims.data())
{
    if(itemBitmap == nullptr)
    {
        this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), true);
    }
}
memBlockItemRangeIterator::memBlockItemRangeIterator(void* data,
                                                     const eleType eType,
                                                     const dimension& dims, 
                                                     const coorRange& range, 
                                                     const dimension& bSp,
                                                     pBitmap itemBitmap)
    : blockItemRangeIterator(data, eType, dims, range, bSp, itemBitmap),
    coorItr(dims.size(), dims.data())
{
    if (itemBitmap == nullptr)
    {
        this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), true);
    }
}
}	// msdb