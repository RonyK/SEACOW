#include <array/chunk.h>
#include <array/memChunkBuffer.h>

namespace msdb
{
chunk::chunk(pChunkDesc desc) : cached_(nullptr), desc_(desc),
serializable(std::make_shared<chunkHeader>())
{
}

chunk::~chunk()
{
	this->free();
}

void chunk::bufferAlloc()
{
	this->free();
	this->makeBuffer();
	this->cached_->bufferAlloc(this->desc_->mSize_);
	this->referenceBufferToBlock();
}

void chunk::bufferAlloc(bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->bufferAlloc(size);
	this->referenceBufferToBlock();
	this->desc_->mSize_ = size;
}

// Deep copy
void chunk::bufferCopy(void* data, bufferSize size)
{
	this->bufferAlloc(size);
	this->cached_->copy(data, size);
	this->desc_->mSize_ = size;
}

void chunk::bufferCopy(pChunk source)
{
	bufferSize size = source->getDesc()->mSize_;
	this->bufferCopy(source->getBuffer()->getData(), size);
}

void chunk::bufferCopy(pBlock source)
{
	bufferSize size = source->getDesc()->mSize_;
	this->bufferCopy(source->getBuffer()->getData(), size);
}

// Copy pointer
void chunk::bufferRef(void* data, bufferSize size)
{
	this->free();
	this->makeBuffer();
	this->cached_->reference(data, size);
	this->referenceBufferToBlock();
	this->desc_->mSize_ = size;
}

void chunk::bufferRef(pBlock source)
{
	bufferSize size = source->getDesc()->mSize_;
	this->bufferRef(source->getBuffer()->getData(), size);
}

//void chunk::bufferCopy(bstream& bs)
//{
//	bufferSize size = bs.capacity();
//	this->bufferAlloc(size);
//	this->cached_->copy(data, size);
//	this->desc_->mSize_ = size;
//}

bool chunk::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}

pChunkBuffer chunk::getBuffer()
{
	return this->cached_;
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
chunk::size_type chunk::getDSize()
{
	return this->desc_->getDimSize();
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
		this->cached_ = nullptr;
	}
}

void chunk::makeAllBlocks()
{
	this->makeBlocks(std::vector<bool>(this->getBlockCapacity(), true));
}

size_t chunk::getBlockCapacity()
{
	return this->blockCapacity_;
}

void chunk::updateToHeader()
{
	auto curHeader = std::static_pointer_cast<chunkHeader>(this->getHeader());
	curHeader->version_ = chunk::chunkHeader::chunk_header_version;
	curHeader->bodySize_ = this->getSerializedSize();
}

void chunk::updateFromHeader()
{
}
}
