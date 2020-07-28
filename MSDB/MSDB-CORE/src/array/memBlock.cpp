#include <array/memBlock.h>

namespace msdb
{
memBlock::memBlock(pBlockDesc desc)
	: block(desc)
{
}

memBlock::~memBlock()
{
}

void memBlock::serialize(bstream& bs)
{
	switch (this->desc_->eType_)
	{
	case eleType::CHAR:
		this->serializeTy<char>(bs);
		break;
	case eleType::INT8:
		this->serializeTy<int8_t>(bs);
		break;
	case eleType::INT16:
		this->serializeTy<int16_t>(bs);
		break;
	case eleType::INT32:
		this->serializeTy<int32_t>(bs);
		break;
	case eleType::INT64:
		this->serializeTy<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->serializeTy<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->serializeTy<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->serializeTy<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->serializeTy<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}

void memBlock::deserialize(bstream& bs)
{
	switch (this->desc_->eType_)
	{
	case eleType::CHAR:
		this->deserializeTy<char>(bs);
		break;
	case eleType::INT8:
		this->deserializeTy<int8_t>(bs);
		break;
	case eleType::INT16:
		this->deserializeTy<int16_t>(bs);
		break;
	case eleType::INT32:
		this->deserializeTy<int32_t>(bs);
		break;
	case eleType::INT64:
		this->deserializeTy<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->deserializeTy<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->deserializeTy<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->deserializeTy<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->deserializeTy<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}

pChunkItemIterator memBlock::getItemIterator()
{
	//void* data, eleType eType, const size_type dSize,
	//	position_t* dims, dim_pointer csP
	return std::make_shared<chunkItemIterator>(this->cached_->getData(),
							 this->desc_->eType_,
							 this->desc_->dims_,
							 this->desc_->sp_);
}

pChunkItemRangeIterator memBlock::getItemRangeIterator(const coorRange& range)
{
	return std::make_shared<chunkItemRangeIterator>(this->cached_->getData(),
								  this->desc_->eType_,
								  this->desc_->dims_,
								  range,
								  this->desc_->sp_);
}
}	// msdb
