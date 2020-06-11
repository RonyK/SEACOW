#include <array/memChunk.h>
#include <system/exceptions.h>
#include <cassert>

namespace msdb
{
memChunk::memChunk()
{

}

bool memChunk::isCompressed() const
{
	return false;
}

//compChunk* memChunk::compress(CompressionMethod cm)
//{
//	switch(cm)
//	{
//	case CompressionMethod::SPIHT:
//		break;
//	case CompressionMethod::CA:
//		break;
//	case CompressionMethod::NONE:
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_CHUNK_CANNOT_COMPRESS));
//	}
//}

memChunkIterator::memChunkIterator(value_t* data, const size_t dSize, position_t* boundary)
	: coorItr(data, dSize, boundary)
{

}
//coorItr& memChunkIterator::operator[](size_type pos)
//{
//	size_type innerPos;
//	return ptr_[this->posToSeq(innerPos)];
//}
}
