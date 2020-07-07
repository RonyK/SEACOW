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

void chunk::alloc()
{
	this->free();
	this->makeBuffer();
	this->cached_->alloc(this->desc_->mSize_);
}

void chunk::alloc(bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->alloc(size);
	this->desc_->mSize_ = size;
}

// Deep copy
void chunk::materializeCopy(void* data, bufferSize size)
{
	this->alloc(size);
	this->cached_->copy(data, size);
	this->desc_->mSize_ = size;
}

// Copy pointer
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
	memChunkItemIterator it(this->cached_->getData(),
						 this->desc_->attrDesc_->type_,
						 this->desc_->dims_.size(),
						 this->desc_->dims_.data(),
						 this->desc_->sp_.data());
	return it;
}
chunkItemRangeIterator chunk::getItemRangeIterator(chunkItemRangeIterator::dim_const_pointer sP, 
												   chunkItemRangeIterator::dim_const_pointer eP)
{
	chunkItemRangeIterator it(this->cached_->getData(),
						   this->desc_->attrDesc_->type_,
						   this->desc_->dims_.size(),
						   this->desc_->dims_.data(),
						   sP, eP,
						   this->desc_->sp_.data());
	return it;
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

coor chunk::getChunkCoor()
{
	return this->desc_->chunkCoor_;
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
	: itemItr(data, eType, dSize, dims), chunkItemItrBase(data, eType, dSize, dims, csP), coorItr(dSize, dims)
{
}
chunkItemRangeIterator::chunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
											   position_t* dims, dim_const_pointer sP, dim_const_pointer eP,
											   dim_pointer csP)
	: itemRangeItr(data, eType, dSize, dims, sP, eP), chunkItemItrBase(data, eType, dSize, dims, csP), coorItr(dSize, dims)
{
}
}
