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
void block::alloc()
{
	this->free();
	this->makeBuffer();
	this->cached_->alloc(this->desc_->mSize_);
}
void block::alloc(bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->alloc(size);
	this->desc_->mSize_ = size;
}
void block::materializeCopy(void* data, bufferSize size)
{
	this->alloc(size);
	this->cached_->copy(data, size);
	this->desc_->mSize_ = size;
}
void block::materializeAssign(void* data, bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->assign(data, size);
	this->desc_->mSize_ = size;
}
bool block::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
void block::free()
{
	if(this->isMaterialized())
	{
		this->cached_ = nullptr;
	}
}
}
