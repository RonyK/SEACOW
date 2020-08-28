#pragma once
#ifndef _MSDB_BLOCKITERATOR_H_
#define _MSDB_BLOCKITERATOR_H_

#include <array/blockContainer.h>
#include <index/bitmap.h>
#include <util/coordinate.h>

namespace msdb
{
class blockIterator;
using pBlockIterator = std::shared_ptr<blockIterator>;

class blockIterator : public coorItr
{
public:
	using self_type = blockIterator;
	using base_type = coorItr;

	using size_type = coorItr::size_type;


public:
	blockIterator(const dimension dims, 
				  blockContainer* blocks, pBitmap blockBitmap,
				  const iterateMode itMode);

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

		if(this->itMode_ == iterateMode::EXIST)
		{
			while (!this->isExist() && !this->isEnd())
			{
				base_type::next();
			}
		}
	}
	virtual void prev()
	{
		// TODO::use STL iterator
		base_type::prev();

		if (this->itMode_ == iterateMode::EXIST)
		{
			while (!this->isExist() && !this->isFront())
			{
				base_type::prev();
			}
		}
	}

	//////////////////////////////
	// Operators
	//////////////////////////////
	// If 'seqPos_' does not match the key of any block in the container,
	// the function throws an 'std::out_of_range' exception.
	virtual pBlock operator*() { return this->blocks_->at(this->seqPos_); }
	virtual pBlock operator->() { return this->blocks_->at(this->seqPos_); }

protected:
	blockContainer* blocks_;
	pBitmap blockBitmap_;
	iterateMode itMode_;
};

class singleBlockIterator : public blockIterator
{
public:
	singleBlockIterator(const dimension dims, pBlock blk, const iterateMode itMode);
	singleBlockIterator(const singleBlockIterator& mit);

public:
	size_type getSeqEnd();
	bool isExist();
	bool isExist(blockId bid);

	virtual void next();
	virtual void prev();

	virtual pBlock operator*() { return (this->seqPos_ == 0) ? this->block_ : throw std::out_of_range("blockIterator seq pos > 0"); }
	virtual pBlock operator->() { return (this->seqPos_ == 0) ? this->block_ : throw std::out_of_range("blockIterator seq pos > 0"); }

protected:
	pBlock block_;
};
}

#endif	// _MSDB_BLOCKITERATOR_H_