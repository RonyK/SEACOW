#include <array/chunkDesc.h>

namespace msdb
{
chunkDesc::chunkDesc()
{
}

chunkDesc::chunkDesc(attributeId id, coor sp, coor ep, chunkSize mSize)
	: id_(id), sp_(sp), ep_(ep), mSize_(mSize), cSize_(mSize), cType_(CompressionMethod::NONE), useCompression_(false)
{
}

chunkDesc::chunkDesc(attributeId id, coor sp, coor ep, chunkSize mSize, chunkSize cSize, CompressionMethod cType)
	: id_(id), sp_(sp), ep_(ep), mSize_(mSize), cSize_(cSize), cType_(cType), useCompression_(true)
{
}

}
