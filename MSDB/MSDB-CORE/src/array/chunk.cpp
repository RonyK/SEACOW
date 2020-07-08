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

void chunk::updateToHeader()
{
	auto curHeader = std::static_pointer_cast<chunkHeader>(this->getHeader());
	curHeader->version_ = chunk::chunkHeader::chunk_header_version;
	curHeader->bodySize_ = this->getSerializedSize();
}

void chunk::updateFromHeader()
{
}

void chunk::serialize(std::ostream& os)
{
	bstream bs;
	switch (this->desc_->attrDesc_->type_)
	{
	case eleType::CHAR:
		this->serialize<char>(bs);
		break;
	case eleType::INT8:
		this->serialize<int8_t>(bs);
		break;
	case eleType::INT16:
		this->serialize<int16_t>(bs);
		break;
	case eleType::INT32:
		this->serialize<int32_t>(bs);
		break;
	case eleType::INT64:
		this->serialize<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->serialize<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->serialize<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->serialize<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->serialize<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
	this->getOutHeader()->serialize(os);
	os.write(bs.data(), bs.capacity());
}

void chunk::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();
	bstream bs;
	bs.resize(this->serializedSize_);
	is.read(bs.data(), this->serializedSize_);
	switch (this->desc_->attrDesc_->type_)
	{
	case eleType::CHAR:
		this->deserialize<char>(bs);
		break;
	case eleType::INT8:
		this->deserialize<int8_t>(bs);
		break;
	case eleType::INT16:
		this->deserialize<int16_t>(bs);
		break;
	case eleType::INT32:
		this->deserialize<int32_t>(bs);
		break;
	case eleType::INT64:
		this->deserialize<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->deserialize<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->deserialize<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->deserialize<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->deserialize<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
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
