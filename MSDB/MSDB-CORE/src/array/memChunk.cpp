#include <array/memChunk.h>

namespace msdb
{
memChunk::memChunk()
{

}

bool memChunk::isCompressed() const
{
	return false;
}

//rawChunkIterator::rawChunkIterator(value_t* data, const size_t dSize, position_t* boundary)
//	: coorItr(data, dSize, boundary)
//{
//
//}
//coorItr& rawChunkIterator::operator[](size_type pos)
//{
//	size_type innerPos;
//	return ptr_[this->posToSeq(innerPos)];
//}
}
