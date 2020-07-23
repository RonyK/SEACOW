#include <array/blockChunk.h>
#include <array/memChunkBuffer.h>

namespace msdb
{
blockChunk::blockChunk(pChunkDesc desc)
	: chunk(desc)
{
}

blockChunk::~blockChunk()
{
}

void blockChunk::makeBuffer()
{
	// TODO :: Replace to blockChunkBuffer
	this->cached_ = std::make_shared<memChunkBuffer>();
}

chunkItemIterator blockChunk::getItemIterator()
{
	blockChunkItemIterator it(this->cached_->getData(),
							this->desc_->attrDesc_->type_,
							this->desc_->dims_.size(),
							this->desc_->dims_.data(),
							this->desc_->sp_.data());
	return it;
}

chunkItemRangeIterator blockChunk::getItemRangeIterator(const coorRange& range)
{
	blockChunkItemRangeIterator it(this->cached_->getData(),
								 this->desc_->attrDesc_->type_,
								 this->desc_->dims_.size(),
								 range,
								 this->desc_->dims_.data(),
								 this->desc_->sp_.data());
	return it;
}

void blockChunk::serialize(std::ostream& os)
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

void blockChunk::deserialize(std::istream& is)
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
blockChunkItemIterator::blockChunkItemIterator(void* data, eleType eType, const size_type dSize,
											   position_t* dims, dim_pointer csP)
	:chunkItemIterator(data, eType, dSize, dims, csP), coordinateIterator<dim_type>(dSize, dims)
{
}
blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
														 dim_const_pointer sP, dim_const_pointer eP,
														 position_t* dims, dim_pointer csP)
	: chunkItemRangeIterator(data, eType, dSize, dims, sP, eP, csP), coorItr(dSize, dims)
{
}
blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
														 const coorRange& range,
														 position_t* dims,
														 dim_pointer csP)
	: chunkItemRangeIterator(data, eType, dSize, dims, range, csP), coorItr(dSize, dims)
{
}
}	// msdb