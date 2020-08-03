
#include <array/blockIterator.h>

namespace msdb
{
// BLOCK ITERATOR
blockIterator::blockIterator(const size_type dSize, dim_const_pointer dims,
							 blockContainer* blocks, iterateMode itMode)
	: coorItr(dSize, dims), blocks_(blocks), itMode_(itMode)
{
	// TODO::blocks_(blocks)
}

blockIterator::blockIterator(const dimension dims,
							 blockContainer* blocks, iterateMode itMode)
	: coorItr(dims), blocks_(blocks), itMode_(itMode)
{
}

blockIterator::blockIterator(const self_type& mit)
	: coorItr(mit), blocks_(mit.blocks_), itMode_(mit.itMode_)
{
}

blockIterator::size_type blockIterator::getSeqEnd()
{
	return 0;
}

bool blockIterator::isExist()
{
	return false;
}

bool blockIterator::isExist(blockId cid)
{
	return false;
}

iterateMode blockIterator::getIterateMode()
{
	return this->itMode_;
}

singleBlockIterator::singleBlockIterator(pBlock blk, iterateMode itMode)
	: blockIterator(dimension(1), nullptr, itMode), block_(blk)
{
}

singleBlockIterator::singleBlockIterator(const singleBlockIterator& mit)
	: blockIterator(mit), block_(mit.block_)
{
}

blockIterator::size_type singleBlockIterator::getSeqEnd()
{
	return 1;
}

bool singleBlockIterator::isExist()
{
	if (this->seqPos_ == 0)
		return true;
	return false;
}

bool singleBlockIterator::isExist(blockId bid)
{
	if (bid == 0)
		return true;
	return false;
}

void singleBlockIterator::next()
{
	if(this->seqPos_ == 0)
	{
		++this->seqPos_;
		this->end_ = true;
		this->front_ = false;
	}
}

void singleBlockIterator::prev()
{
	if(this->seqPos_ == 1)
	{
		--this->seqPos_;
		this->front_ = true;
		this->end_ = false;
	}
}
}