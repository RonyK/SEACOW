#include <array/chunkIterator.h>

namespace msdb
{
chunkIterator::chunkIterator(const size_type dSize, dim_const_pointer dims,
							 chunkContainer* chunks, iterateMode itMode)
	: coorItr(dSize, dims), chunks_(chunks), itMode_(itMode)
{
}

chunkIterator::chunkIterator(const self_type& mit)
	: coorItr(mit), chunks_(mit.chunks_), itMode_(mit.itMode_)
{
}

chunkIterator::size_type chunkIterator::getSeqEnd()
{
	return this->chunks_->size();
}

bool chunkIterator::isExist()
{
	return this->chunks_->find(this->seqPos_) != this->chunks_->end();
}

bool chunkIterator::isExist(chunkId cid)
{
	return this->chunks_->find(cid) != this->chunks_->end();
}

iterateMode chunkIterator::getIterateMode()
{
	return this->itMode_;
}
}	// msdb