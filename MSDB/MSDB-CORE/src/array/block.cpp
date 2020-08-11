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
void block::unreference()
{
	this->cached_ = nullptr;
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
