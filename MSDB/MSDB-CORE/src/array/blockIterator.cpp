
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

}