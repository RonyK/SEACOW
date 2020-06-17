#include <array/chunk.h>

namespace msdb
{
chunk::chunk(pChunkDesc desc) : cached_(nullptr), desc_(desc)
{

}
chunk::~chunk()
{
	this->free();
}

void chunk::materialize()
{
	this->free();
	this->cached_ = new chunkBuffer();
	
	this->desc_->mSize_ = 0;
}

void chunk::materialize(bufferSize size)
{
	this->free();
	this->cached_ = new chunkBuffer();
	this->cached_->alloc(size);

	this->desc_->mSize_ = size;
}

void chunk::materializeCopy(void* data, bufferSize size)
{
	this->free();
	this->materialize(size);
	this->cached_->copy(data, size);
}

void chunk::materializeAssign(void* data, bufferSize size)
{
	this->materialize();
	this->cached_->assign(data, size);
}

bool chunk::isMaterialized() const
{
	if(this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
chunkItemIterator chunk::getItemIterator()
{
	return chunkItemIterator(this->cached_->getData(),
							 this->desc_->attrDesc_->type_,
							 this->desc_->dims_.size(),
							 this->desc_->dims_.data(),
							 this->desc_->sp_.data());
}
chunkId chunk::getId() const
{
	return this->desc_->id_;
}
const pChunkDesc chunk::getDesc() const
{
	return this->desc_;
}
void chunk::free()
{
	if (this->isMaterialized())
	{
		delete this->cached_;
	}
}
chunkItemIterator::chunkItemIterator(void* data, eleType eType, const size_type dSize, position_t* dims, dim_pointer csP)
	: itemItr(data, eType, dSize, dims)
{
	this->csP_ = new dim_type[dSize];
	this->memcpyDim(this->csP_, csP);
}

chunkItemIterator::coordinate_type chunkItemIterator::coorOut2In(coordinate_type& out)
{
	coordinate_type in(this->dSize());
	for (dimensionId d = 0; d < this->dSize(); d++)
	{
		in[d] = out[d] - this->csP_[d];
	}

	return in;
}

chunkItemIterator::coordinate_type chunkItemIterator::coorIn2Out(coordinate_type& in)
{
	coordinate_type out(this->dSize());
	for (dimensionId d = 0; d < this->dSize(); d++)
	{
		out[d] = in[d] + this->csP_[d];
	}
	return out;
}

chunkItemIterator::coordinate_type chunkItemIterator::coorIn2Out()
{
	return this->coorIn2Out(this->coor_);
}

chunkItemIterator::coordinate_type chunkItemIterator::ceP()
{
	coordinate_type ceP(this->dSize());
	for (dimensionId d = 0; d < this->dSize(); d++)
	{
		ceP[d] = this->csP_[d] + this->dims_[d];
	}

	return ceP;
}

chunkItemIterator::coordinate_type chunkItemIterator::outCoor()
{
	coordinate_type out(this->dSize());
	for (dimensionId d = 0; d < this->dSize(); d++)
	{
		out[d] = this->coor_[d] + this->csP_[d];
	}
	return out;
}

chunkItemIterator::coordinate_type chunkItemIterator::innerCoor()
{
	return this->coor_;
}
}
