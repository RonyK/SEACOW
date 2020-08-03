#include <array/blockChunk.h>
#include <array/memChunkBuffer.h>

namespace msdb
{
blockChunk::blockChunk(pChunkDesc desc)
	: chunk(desc)
{
}

blockChunk::~blockChunk()
{
}

void blockChunk::makeBuffer()
{
	// TODO :: Replace to blockChunkBuffer
	this->cached_ = std::make_shared<memChunkBuffer>();
}

size_t blockChunk::getBlockCapacity()
{
	auto bItr = this->getBlockIterator();
	return bItr->getCapacity();
}

pBlock blockChunk::getBlock(blockId bId)
{
	return this->blocks_[bId];
}

blockId blockChunk::getBlockId(pBlockDesc bDesc)
{
	return this->getBlockIdFromBlockCoor(bDesc->sp_);
}

blockId blockChunk::getBlockIdFromItemCoor(coor& itemCoor)
{
	auto blockCoor = this->itemCoorToBlockCoor(itemCoor);
	return this->getBlockIdFromBlockCoor(blockCoor);
}

blockId blockChunk::getBlockIdFromBlockCoor(coor& chunkCoor)
{
	//auto bItr = this->getBlockIterator();
	//return bItr.coorToSeq(chunkCoor);
	return 0;
}

coor blockChunk::itemCoorToBlockCoor(coor& itemCoor)
{
	coor blockCoor = itemCoor;
	pBlockChunkDesc bDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
	blockCoor /= bDesc->blockDims_;
	return blockCoor;
}

pBlockIterator blockChunk::getBlockIterator(iterateMode itMode)
{
	auto bChunkDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
	return std::make_shared<blockIterator>(bChunkDesc->blockDims_, &this->blocks_, itMode);
}

pChunkItemIterator blockChunk::getItemIterator()
{
	return std::make_shared<blockChunkItemIterator>(this->cached_->getData(),
													this->desc_->attrDesc_->type_,
													this->desc_->dims_,
													this->desc_->sp_,
													this->getBlockIterator());
}

pChunkItemRangeIterator blockChunk::getItemRangeIterator(const coorRange& range)
{
	return std::make_shared<blockChunkItemRangeIterator>(this->cached_->getData(),
														 this->desc_->attrDesc_->type_,
														 this->desc_->dims_,
														 range,
														 this->desc_->sp_,
														 this->getBlockIterator());
}

void blockChunk::setBlock(pBlock b)
{
	this->blocks_[b->getId()] = b;
}

void blockChunk::flush()
{
	this->blocks_.clear();
	// TODO:: Flush cached
}

void blockChunk::serialize(std::ostream& os)
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

void blockChunk::deserialize(std::istream& is)
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

// BLOCKCHUNKITEMITERATOR
blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const size_type dSize,
											   dim_const_pointer dims,
											   dim_const_pointer csP,
											   pBlockIterator bItr)
	: chunkItemIterator(data, eType, dSize, dims, csP),
	coorItr(dSize, dims),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	// if iterMode all
	if (bItr_->isExist())
	{
		auto it = (*bItr_)->getItemIterator();
		//this->curBlockItemItr_ = std::make_shared<chunkItemIterator>
	}
	// TODO::set cur block item itr
}




blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const dimension dims,
											   const dimension csP,
											   pBlockIterator bItr)
	: chunkItemIterator(data, eType, dims, csP),
	coorItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	// TODO::set cur block item itr
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
	}else
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

blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, const eleType eType, const size_type dSize,
														 dim_const_pointer dims,
														 dim_const_pointer sP, dim_const_pointer eP,
														 dim_const_pointer csP, pBlockIterator bItr)
	: chunkItemRangeIterator(data, eType, dSize, dims, sP, eP, csP),
	coorItr(dSize, dims),
	bItr_(bItr)
{
}
blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType,
														 const dimension dims,
														 const coorRange& range,
														 const dimension csP, pBlockIterator bItr)
	: chunkItemRangeIterator(data, eType, dims, range, csP),
	coorItr(dims.size(), dims.data()),
	bItr_(bItr)
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
				
				if(bRange < qRange)
				{
					this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
				}else
				{
					auto sp = getOutsideCoor(bDesc->sp_, this->sP_);
					auto ep = getInsideCoor(bDesc->ep_, this->eP_);
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

element blockChunkItemRangeIterator::getAt(position_t pos)
{
	return this->curBlockItemItr_->getAt(pos);
}

element blockChunkItemRangeIterator::operator*()
{
	return (**this->curBlockItemItr_);
}
}	// msdb