#pragma once
#ifndef _MSDB_BLOCKITERATOR_H_
#define _MSDB_BLOCKITERATOR_H_

#include <util/coordinate.h>
#include <array/blockId.h>
#include <array/blockContainer.h>

namespace msdb
{
class blockIterator : public coorItr
{
public:
	using self_type = blockIterator;
	using base_type = coorItr;

	using size_type = coorItr::size_type;


public:
	blockIterator(const size_type dSize, dim_const_pointer dims,
				  blockContainer* blocks, iterateMode itMode);

	blockIterator(const dimension dims, blockContainer* blocks, iterateMode itMode);

	blockIterator(const self_type& mit);

public:
	size_type getSeqEnd();
	bool isExist();
	bool isExist(blockId cid);
	iterateMode getIterateMode();

	//////////////////////////////
	// Iterating
	//////////////////////////////
	virtual void next()
	{
		// TODO::use STL iterator
		base_type::next();

		while (!this->isExist() && !this->isEnd())
		{
			base_type::next();
		}
	}
	virtual void prev()
	{
		// TODO::use STL iterator
		base_type::prev();

		while (!this->isExist() && !this->isFront())
		{
			base_type::prev();
		}
	}

	//////////////////////////////
	// Operators
	//////////////////////////////
	//pBlock operator*() { return this->blocks_->at(this->seqPos_); }
	//pBlock operator->() { return this->blocks_->at(this->seqPos_); }

protected:
	blockContainer* blocks_;
	iterateMode itMode_;
};
}

#endif	// _MSDB_BLOCKITERATOR_H_