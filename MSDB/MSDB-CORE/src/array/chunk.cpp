#include <array/chunk.h>
#include <array/memChunkBuffer.h>

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
	this->makeBuffer();
	this->cached_->alloc(this->desc_->mSize_);
}

void chunk::materialize(bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->alloc(size);
	this->desc_->mSize_ = size;
}

void chunk::materializeCopy(void* data, bufferSize size)
{
	this->free();
	this->materialize(size);
	this->cached_->copy(data, size);
	this->desc_->mSize_ = size;
}

void chunk::materializeAssign(void* data, bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->assign(data, size);
	this->desc_->mSize_ = size;
}

bool chunk::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
chunkItemIterator chunk::getItemIterator()
{
	return memChunkItemIterator(this->cached_->getData(),
								this->desc_->attrDesc_->type_,
								this->desc_->dims_.size(),
								this->desc_->dims_.data(),
								this->desc_->sp_.data());
}
chunkId chunk::getId() const
{
	return this->desc_->id_;
}

void chunk::setId(chunkId id)
{
	this->desc_->id_ = id;
}

const pChunkDesc chunk::getDesc() const
{
	return this->desc_;
}
chunk::size_type chunk::numCells()
{
	size_type output = 1;
	for (dimensionId d = 0; d < this->desc_->dims_.size(); d++)
	{
		output *= this->desc_->dims_[d];
	}
	return output;
}
void chunk::print()
{
	switch (this->desc_->attrDesc_->type_)
	{
	case eleType::BOOL:
		return this->printImp<bool>();
	case eleType::CHAR:
		return this->printImp<char>();
	case eleType::INT8:
		return this->printImp<int8_t>();
	case eleType::INT16:
		return this->printImp<int16_t>();
	case eleType::INT32:
		return this->printImp<int32_t>();
	case eleType::INT64:
		return this->printImp<int64_t>();
	case eleType::UINT8:
		return this->printImp<uint8_t>();
	case eleType::UINT16:
		return this->printImp<uint16_t>();
	case eleType::UINT32:
		return this->printImp<uint32_t>();
	case eleType::UINT64:
		return this->printImp<uint64_t>();
	case eleType::FLOAT:
		return this->printImp<float>();
	case eleType::DOUBLE:
		return this->printImp<double>();
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}
void chunk::free()
{
	if (this->isMaterialized())
	{
		delete this->cached_;
	}
}
void chunk::makeBuffer()
{
	this->cached_ = new memChunkBuffer();
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
