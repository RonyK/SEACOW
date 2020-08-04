#include <compression/seChunk.h>

#include <array/chunk.h>

namespace msdb
{
seChunk::seChunk(pChunkDesc desc)
	: memBlockChunk(desc), level_(0), 
	bandId_(INVALID_CHUNK_ID), sourceChunkId_(INVALID_CHUNK_ID)
{
	//auto chunkDim = this->desc_->dims_;
	//auto blockDim = std::static_pointer_cast<blockChunkDesc>(this->desc_)->blockDims_;
	//auto blockSpace = chunkDim / blockDim;
	//this->rBitFromMMT.resize(blockSpace.area());
	//this->rBitFromDelta.resize(blockSpace.area());
}

inline seChunk::~seChunk()
{
}

size_t seChunk::getLevel()
{
	return this->level_;
}
chunkId seChunk::getBandId()
{
	return this->bandId_;
}
//chunkId seChunk::getTileId()
//{
//	return this->tileId_;
//}
chunkId seChunk::getSourceChunkId()
{
	return this->sourceChunkId_;
}
void seChunk::setLevel(size_t level)
{
	this->level_ = level;
}
void seChunk::setBandId(chunkId bid)
{
	this->bandId_ = bid;
}
//void seChunk::setTileId(chunkId tid)
//{
//	this->tileId_ = tid;
//}
void seChunk::setSourceChunkId(chunkId cid)
{
	this->sourceChunkId_ = cid;
}

void seChunk::serialize(std::ostream& os)
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

void seChunk::deserialize(std::istream& is)
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