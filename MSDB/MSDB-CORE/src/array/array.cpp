#include <array/array.h>

namespace msdb
{
arrayBase::arrayBase(arrayDesc& desc)
{
	this->desc_ = desc;
}
arrayDesc arrayBase::getDesc()
{
	return this->desc_;
}
arrayIterator arrayBase::getIterator()
{
	return arrayIterator(this->desc_.dims_.size(), 
						 this->desc_.dims_.getDims().data());
}
arrayIterator::arrayIterator(const size_type dSize, dim_const_pointer dims)
	: coorItr(dSize, dims)
{
}
arrayIterator::arrayIterator(const self_type& mit)
	: coorItr(mit)
{
}
}
