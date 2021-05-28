#include <stdafx.h>
#include <compression/seHuffmanChunk.h>
#include <compression/seHuffmanBlock.h>
#include <util/ioutil.h>

namespace msdb
{
seHuffmanChunk::seHuffmanChunk(pChunkDesc desc)
	: seChunk(desc)
{
}

seHuffmanChunk::~seHuffmanChunk()
{
}

pBlock seHuffmanChunk::makeBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	if (this->blocks_[bId] == nullptr)
	{
		// Make new one
		auto desc = this->getBlockDesc(bId);
		auto blockObj = std::make_shared<seHuffmanBlock>(desc);
		this->insertBlock(blockObj);
	}

	return this->blocks_[bId];
}

void seHuffmanChunk::serialize(std::ostream& os)
{
	bstream bs;
	switch (this->desc_->attrDesc_->type_)
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
		this->serializeTy<int8_t>(bs);
		break;
	case eleType::UINT16:
		this->serializeTy<int16_t>(bs);
		break;
	case eleType::UINT32:
		this->serializeTy<int32_t>(bs);
		break;
	case eleType::UINT64:
		this->serializeTy<int64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}

	this->serializedSize_ = bs.capacity();
	this->getOutHeader()->serialize(os);
	os.write(bs.data(), bs.capacity());
}

void seHuffmanChunk::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	bstream bs;
	bs.resize(this->serializedSize_);
	is.read(bs.data(), this->serializedSize_);

	switch (this->desc_->attrDesc_->type_)
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
		this->deserializeTy<int8_t>(bs);
		break;
	case eleType::UINT16:
		this->deserializeTy<int16_t>(bs);
		break;
	case eleType::UINT32:
		this->deserializeTy<int32_t>(bs);
		break;
	case eleType::UINT64:
		this->deserializeTy<int64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}

std::vector<iFixedHuffmanCoder*> seHuffmanChunk::fixedHuffmanCoders = {
	fixedHuffmanCoder<1>::instance(),
	fixedHuffmanCoder<1>::instance(),
	fixedHuffmanCoder<2>::instance(), 
	fixedHuffmanCoder<3>::instance(),
	fixedHuffmanCoder<4>::instance(),
	fixedHuffmanCoder<5>::instance(),
	fixedHuffmanCoder<6>::instance(),
	fixedHuffmanCoder<7>::instance(),
	fixedHuffmanCoder<8>::instance()
};
}	// msdb