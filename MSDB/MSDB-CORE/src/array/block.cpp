#include <array/block.h>

namespace msdb
{
block::block(pBlockDesc desc)
	: desc_(desc)
{

}
block::~block()
{

}
blockId block::getId()
{
	return this->desc_->id_;
}
pBlockDesc block::getDesc()
{
	return this->desc_;
}
dimensionId block::getDSize()
{
	return this->desc_->dims_.size();
}
void block::unreference()
{
	this->cached_ = nullptr;
}
void block::copy(pBlock srcBlock)
{
	assert(srcBlock->getDesc()->mSize_ <= this->getDesc()->mSize_);
	assert(this->isMaterialized() == true);

	auto offset = this->getId() * this->getDesc()->mSize_;
	this->cached_->copy(srcBlock->getBuffer()->getData(), offset, srcBlock->getDesc()->mSize_);
}
bool block::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
pBlockBuffer block::getBuffer()
{
	return this->cached_;
}
}
