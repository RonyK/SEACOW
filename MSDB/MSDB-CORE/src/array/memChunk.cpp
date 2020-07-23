#include <array/memChunk.h>
#include <array/memChunkBuffer.h>

namespace msdb
{
memChunk::memChunk(pChunkDesc desc)
	: chunk(desc)
{
}

memChunk::~memChunk()
{
}

void memChunk::makeBuffer()
{
	this->cached_ = std::make_shared<memChunkBuffer>();
}

chunkItemIterator memChunk::getItemIterator()
{
	memChunkItemIterator it(this->cached_->getData(),
							this->desc_->attrDesc_->type_,
							this->desc_->dims_.size(),
							this->desc_->dims_.data(),
							this->desc_->sp_.data());
	return it;
}
chunkItemRangeIterator memChunk::getItemRangeIterator(const coorRange& range)
{
	memChunkItemRangeIterator it(this->cached_->getData(),
								 this->desc_->attrDesc_->type_,
								 this->desc_->dims_.size(),
								 range,
								 this->desc_->dims_.data(),
								 this->desc_->sp_.data());
	return it;
}

void memChunk::serialize(std::ostream& os)
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

void memChunk::deserialize(std::istream& is)
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
}	// msdb