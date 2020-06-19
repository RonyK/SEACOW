#include <array/chunkDesc.h>

namespace msdb
{
extern const chunkSize INVALID_CHUNK_SIZE = static_cast<chunkSize>(~0);

chunkDesc::chunkDesc()
{
	// TODO::Initialize
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims,
					 const coor sp, const coor ep, const chunkSize mSize)
	: id_(id), attrDesc_(attrDesc), dims_(dims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(mSize), 
	cType_(CompressionMethod::NONE), useCompression_(false)
{
	if (this->mSize_ == INVALID_CHUNK_SIZE)
	{
		this->initPhysicalChunkSizeFromDims();
	}
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims,
					 const coor sp, const coor ep,
					 const chunkSize mSize, const chunkSize cSize,
					 const CompressionMethod cType)
	: id_(id), attrDesc_(attrDesc), dims_(dims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(cSize),
	cType_(cType), useCompression_(true)
{
	if(this->mSize_ == INVALID_CHUNK_SIZE)
	{
		this->initPhysicalChunkSizeFromDims();
	}
}

chunkDesc::chunkDesc(const chunkDesc& mit)
	: id_(mit.id_), attrDesc_(std::make_shared<attributeDesc>(*(mit.attrDesc_))),
	mSize_(mit.mSize_), cSize_(mit.cSize_), useCompression_(mit.useCompression_),
	cType_(mit.cType_), dims_(mit.dims_), sp_(mit.sp_), ep_(mit.ep_)
{
}

void chunkDesc::setDim(dimensionId dId, position_t value)
{
	this->dims_[dId] = value;
	this->initPhysicalChunkSizeFromDims();
}

void chunkDesc::initPhysicalChunkSizeFromDims()
{
	this->mSize_ = this->attrDesc_->typeSize_;
	for(dimensionId d = 0; d < this->dims_.size(); d++)
	{
		this->mSize_ *= this->dims_[d];
	}

	if(this->cSize_ != INVALID_CHUNK_SIZE && this->cSize_ > this->mSize_)
	{
		this->cSize_ = this->mSize_;
	}
}

}
