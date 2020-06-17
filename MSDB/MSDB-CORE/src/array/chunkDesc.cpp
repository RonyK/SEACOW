#include <array/chunkDesc.h>

namespace msdb
{
chunkDesc::chunkDesc()
{
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims,
					 const coor sp, const coor ep, const chunkSize mSize)
	: id_(id), attrDesc_(attrDesc), dims_(dims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(mSize), 
	cType_(CompressionMethod::NONE), useCompression_(false)
{
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims,
					 const coor sp, const coor ep,
					 const chunkSize mSize, const chunkSize cSize,
					 const CompressionMethod cType)
	: id_(id), attrDesc_(attrDesc), dims_(dims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(cSize),
	cType_(cType), useCompression_(true)
{
}

}
