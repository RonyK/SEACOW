#include <array/blockChunk.h>
#include <array/memChunkBuffer.h>
#include <array/memBlock.h>

namespace msdb
{
memBlockChunk::memBlockChunk(pChunkDesc desc)
	: chunk(desc)
{
	this->blockCapacity_ = this->desc_->getBlockSpace().area();
	this->blocks_.resize(this->getBlockCapacity(), nullptr);
}

memBlockChunk::~memBlockChunk()
{
}

void memBlockChunk::makeBuffer()
{
	this->cached_ = std::make_shared<memChunkBuffer>();
}

pBlock memBlockChunk::makeBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	if (this->blocks_[bId] == nullptr)
	{
		auto desc = this->getBlockDesc(bId);
		auto blockObj = std::make_shared<memBlock>(desc);
		this->insertBlock(blockObj);
		return blockObj;
	}
	return this->blocks_[bId];
}

void memBlockChunk::insertBlock(pBlock inBlock)
{
	assert(this->blockCapacity_ > inBlock->getId());
	this->blocks_[inBlock->getId()] = inBlock;
	this->blockBitmap_.setExist(inBlock->getId());
}

void memBlockChunk::referenceBufferToBlock(blockId bId)
{
	// Reference block buffers to the chunk buffer
	if (this->blocks_[bId])
	{
		bufferSize mSizeBlock = this->blocks_[bId]->getDesc()->mSize_;
		this->blocks_[bId]->linkToChunkBuffer(
			(char*)this->cached_->getData() + (bId * mSizeBlock),
			mSizeBlock);
	}
}

pBlock memBlockChunk::getBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	return this->blocks_[bId];
}

pBlockIterator memBlockChunk::getBlockIterator(const iterateMode itMode)
{
	return std::make_shared<blockIterator>(this->desc_->getBlockSpace(),
										   &this->blocks_, &this->blockBitmap_,
										   itMode);
}

pChunkItemIterator memBlockChunk::getItemIterator()
{
	return std::make_shared<blockChunkItemIterator>(this->cached_->getData(),
													this->desc_->attrDesc_->type_,
													this->desc_->dims_,
													this->desc_->sp_,
													this->getBlockIterator());
}

pChunkItemRangeIterator memBlockChunk::getItemRangeIterator(const coorRange& range)
{
	return std::make_shared<blockChunkItemRangeIterator>(this->cached_->getData(),
														 this->desc_->attrDesc_->type_,
														 this->desc_->dims_,
														 range,
														 this->desc_->sp_,
														 this->getBlockIterator());
}

void memBlockChunk::serialize(std::ostream& os)
{
	bstream bs;
	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		(*it)->serialize(bs);
		++(*it);
	}
	this->serializedSize_ = bs.capacity();

	this->getOutHeader()->serialize(os);
	os.write(bs.data(), bs.capacity());
}

void memBlockChunk::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	bstream bs;
	bs.resize(this->serializedSize_);
	is.read(bs.data(), this->serializedSize_);

	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		(*it)->deserialize(bs);
		++(*it);
	}
}

blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const dimension& dims,
											   const dimension& csP,
											   pBlockIterator bItr)
	: chunkItemIterator(data, eType, dims, csP),
	coorItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	this->initBlockItemItr();
}

// Only visits exist blocks
void blockChunkItemIterator::next()
{
	if (this->bItr_->isEnd() || this->curBlockItemItr_ == nullptr)	return;
	this->front_ = false;

	if (this->curBlockItemItr_->isEnd())
	{
		do
		{
			++(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
				this->curBlockItemItr_->moveToStart();
				this->moveToStart();

				break;
			}
		} while (!this->bItr_->isEnd());

		if (this->bItr_->isEnd())
		{
			this->end_ = true;
		}
	} else
	{
		base_type::next();
		this->curBlockItemItr_->next();
	}
}

void blockChunkItemIterator::prev()
{
	if (this->curBlockItemItr_ == nullptr) return;
	if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())	return;
	this->end_ = false;

	if (this->curBlockItemItr_->isFront())
	{
		do
		{
			--(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
				this->curBlockItemItr_->moveToLast();
				this->moveToLast();

				break;
			}
		} while (!this->bItr_->isFront());

		if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())
		{
			this->front_ = true;
		}
	} else
	{
		base_type::prev();
		this->curBlockItemItr_->prev();
	}
}

element blockChunkItemIterator::getAt(position_t pos)
{
	return this->curBlockItemItr_->getAt(pos);
}

element blockChunkItemIterator::operator*()
{
	return (**this->curBlockItemItr_);
}

void blockChunkItemIterator::initBlockItemItr()
{
	while (!this->bItr_->isExist() && !this->bItr_->isEnd())
	{
		++(*this->bItr_);
	}
	if (this->bItr_->isExist())
	{
		this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
	}
}

blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType,
														 const dimension& dims,
														 const coorRange& range,
														 const dimension& csP,
														 pBlockIterator bItr)
	: chunkItemRangeIterator(data, eType, dims, range, csP),
	coorItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
}

void blockChunkItemRangeIterator::next()
{
	if (this->bItr_->isEnd() || this->curBlockItemItr_ == nullptr)	return;
	this->front_ = false;

	if (this->curBlockItemItr_->isEnd())
	{
		do
		{
			++(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				auto bDesc = (**this->bItr_)->getDesc();
				auto bRange = coorRange(bDesc->sp_, bDesc->ep_);
				auto qRange = coorRange(this->sP_, this->eP_);

				if (bRange < qRange)
				{
					// Inside, full scan
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(coorRange(bDesc->sp_, bDesc->ep_));
				} else
				{
					// Intersect
					auto sp = getOutsideCoor(bDesc->sp_, this->sP_);
					auto ep = getInsideCoor(bDesc->ep_, this->eP_);
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(coorRange(sp, ep));
				}

				this->curBlockItemItr_->moveToStart();
				this->moveToStart();

				break;
			}
		} while (!this->bItr_->isEnd());

		if (this->bItr_->isEnd())
		{
			this->end_ = true;
		}
	} else
	{
		base_type::next();
		this->curBlockItemItr_->next();
	}
}

void blockChunkItemRangeIterator::prev()
{
	if (this->curBlockItemItr_ == nullptr) return;
	if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())	return;
	this->end_ = false;

	if (this->curBlockItemItr_->isFront())
	{
		do
		{
			--(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				auto bDesc = (**this->bItr_)->getDesc();
				auto bRange = coorRange(bDesc->sp_, bDesc->ep_);
				auto qRange = coorRange(this->sP_, this->eP_);

				if (bRange < qRange)
				{
					// Inside, full scan
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(coorRange(bDesc->sp_, bDesc->ep_));
				} else
				{
					// Intersect
					auto sp = getOutsideCoor(bDesc->sp_, this->sP_);
					auto ep = getInsideCoor(bDesc->ep_, this->eP_);
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(coorRange(sp, ep));
				}

				this->curBlockItemItr_->moveToLast();
				this->moveToLast();

				break;
			}
		} while (!this->bItr_->isFront());

		if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())
		{
			this->front_ = true;
		}
	} else
	{
		base_type::prev();
		this->curBlockItemItr_->prev();
	}
}

element blockChunkItemRangeIterator::getAt(position_t pos)
{
	return this->curBlockItemItr_->getAt(pos);
}

element blockChunkItemRangeIterator::operator*()
{
	return (**this->curBlockItemItr_);
}
}	// msdb