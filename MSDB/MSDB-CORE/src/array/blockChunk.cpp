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
	this->makeAllBlocks();
}

memBlockChunk::~memBlockChunk()
{
}

void memBlockChunk::makeBuffer()
{
	this->cached_ = std::make_shared<memChunkBuffer>();
}

void memBlockChunk::makeBlocks(std::vector<bool> bitmap)
{
	pChunkDesc bcDesc = std::static_pointer_cast<chunkDesc>(this->desc_);

	blockId capacity = this->getBlockCapacity();
	bufferSize mSizeBlock = bcDesc->blockDims_.area();
	coorItr bCoorItr(bcDesc->getBlockSpace());

	for(blockId bid = 0; bid < capacity; ++bid, ++bCoorItr)
	{
		if(bitmap.size() > bid && bitmap[bid])
		{
			coor blockCoor = bCoorItr.coor();
			coor blockSp = blockCoor * bcDesc->blockDims_;
			coor blockEp = blockSp + bcDesc->blockDims_;
			coorRange bRange = coorRange(blockSp, blockEp);

			// TODO::block bitmap
			pBlockDesc bDesc = std::make_shared<blockDesc>(
				bid,				// blockId
				this->desc_->attrDesc_->type_,	// eType
				bcDesc->blockDims_,	// dims
				blockSp, blockEp,	// sp, ep
				mSizeBlock);		// mSize
			this->blocks_[bid] = std::make_shared<memBlock>(bDesc);
		}else
		{
			this->blocks_[bid] = nullptr;
		}
	}
}

void memBlockChunk::referenceBufferToBlock()
{
	// Reference block buffers to the chunk buffer
	blockId capacity = this->getBlockCapacity();
	bufferSize mSizeBlock = this->desc_->getBlockDim().area();
	for (blockId bid = 0; bid < capacity; ++bid)
	{
		// TODO:: use block bitmap
		if (this->blocks_[bid])
		{
			this->blocks_[bid]->reference(
				(char*)this->cached_->getData() + (bid * mSizeBlock),
				mSizeBlock);
		}
	}
}

pBlock memBlockChunk::getBlock(blockId bId)
{
	return this->blocks_[bId];
}

blockId memBlockChunk::getBlockId(pBlockDesc bDesc)
{
	return this->getBlockIdFromBlockCoor(bDesc->sp_);
}

blockId memBlockChunk::getBlockIdFromItemCoor(coor& itemCoor)
{
	auto blockCoor = this->itemCoorToBlockCoor(itemCoor);
	return this->getBlockIdFromBlockCoor(blockCoor);
}

blockId memBlockChunk::getBlockIdFromBlockCoor(coor& chunkCoor)
{
	//auto bItr = this->getBlockIterator();
	//return bItr.coorToSeq(chunkCoor);
	return 0;
}

coor memBlockChunk::itemCoorToBlockCoor(coor& itemCoor)
{
	coor blockCoor = itemCoor;
	//pChunkDesc bDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
	blockCoor /= this->desc_->getBlockDim();
	return blockCoor;
}

pBlockIterator memBlockChunk::getBlockIterator(iterateMode itMode)
{
	//auto bChunkDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
	return std::make_shared<blockIterator>(this->desc_->getBlockSpace(), &this->blocks_, itMode);
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

void memBlockChunk::setBlock(pBlock b)
{
	this->blocks_[b->getId()] = b;
}

void memBlockChunk::flush()
{
	this->blocks_.clear();
	// TODO:: Flush cached
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
				
				if(bRange < qRange)
				{
					// Inside, full scan
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(coorRange(bDesc->sp_, bDesc->ep_));
				}else
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