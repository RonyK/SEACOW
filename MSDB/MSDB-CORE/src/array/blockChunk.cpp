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

//blockIterator blockChunk::getBlockIterator()
//{
//	iterateMode itMode = iterateMode::ALL;
//	pBlockChunkDesc bDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
//	return blockIterator(bDesc->blockDims_, itMode);
//}

blockIterator blockChunk::getBlockIterator(iterateMode itMode)
{
	auto bChunkDesc = std::static_pointer_cast<blockChunkDesc>(this->desc_);
	return blockIterator(bChunkDesc->blockDims_, &this->blocks_, itMode);
}

chunkItemIterator blockChunk::getItemIterator()
{
	blockChunkItemIterator it(this->cached_->getData(),
							  this->desc_->attrDesc_->type_,
							  this->desc_->dims_,
							  this->desc_->sp_,
							  this->getBlockIterator());
	return it;
}

chunkItemRangeIterator blockChunk::getItemRangeIterator(const coorRange& range)
{
	blockChunkItemRangeIterator it(this->cached_->getData(),
								   this->desc_->attrDesc_->type_,
								   this->desc_->dims_,
								   range,
								   this->desc_->sp_,
								   this->getBlockIterator());
	return it;
}

void blockChunk::flush()
{
	this->blocks_.clear();
}

void blockChunk::serialize(std::ostream& os)
{
	bstream bs;
	auto it = this->getBlockIterator();
	while (!it.isEnd())
	{
		(*it)->serialize(bs);
		++it;
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
	while (!it.isEnd())
	{
		(*it)->deserialize(bs);
		++it;
	}
}

// BLOCKCHUNKITEMITERATOR
blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const size_type dSize,
											   dim_const_pointer dims,
											   dim_const_pointer csP, 
											   blockIterator bItr)
	: chunkItemIterator(data, eType, dSize, dims, csP), 
	coorItr(dSize, dims),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	// TODO::set cur block item itr
}

blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const dimension dims,
											   const dimension csP,
											   blockIterator bItr)
	: chunkItemIterator(data, eType, dims, csP),
	coorItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	// TODO::set cur block item itr
}

blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, const eleType eType, const size_type dSize,
														 dim_const_pointer dims,
														 dim_const_pointer sP, dim_const_pointer eP,
														 dim_const_pointer csP, blockIterator bItr)
	: chunkItemRangeIterator(data, eType, dSize, dims, sP, eP, csP), 
	coorItr(dSize, dims), 
	bItr_(bItr)
{
}
blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType, 
														 const dimension dims,
														 const coorRange& range,
														 const dimension csP, blockIterator bItr)
	: chunkItemRangeIterator(data, eType, dims, range, csP), 
	coorItr(dims.size(), dims.data()),
	bItr_(bItr)
{
}
}	// msdb