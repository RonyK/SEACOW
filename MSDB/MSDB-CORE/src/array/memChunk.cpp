#include <array/memChunk.h>
#include <array/memChunkBuffer.h>
#include <array/memChunkItemIterator.h>

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

pChunkItemIterator memChunk::getItemIterator()
{
	return std::make_shared<memChunkItemIterator>(this->cached_->getData(),
							this->desc_->attrDesc_->type_,
							this->desc_->dims_,
							this->desc_->sp_);
}
pChunkItemRangeIterator memChunk::getItemRangeIterator(const coorRange& range)
{
	return std::make_shared<memChunkItemRangeIterator>(this->cached_->getData(),
								 this->desc_->attrDesc_->type_,
								 this->desc_->dims_,
								 range,
								 this->desc_->sp_);
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

	this->serializedSize_ = bs.capacity();
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