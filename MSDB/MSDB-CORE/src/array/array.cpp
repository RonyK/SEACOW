#include <array/array.h>
#include <util/math.h>
#include <cmath>

namespace msdb
{
arrayBase::arrayBase(pArrayDesc desc)
{
	this->desc_ = desc;
}
pArrayDesc arrayBase::getDesc()
{
	return this->desc_;
}
chunkIterator arrayBase::getChunkIterator(iterateMode itMode)
{
	return chunkIterator(this->desc_->dimDescs_.size(), 
						 this->desc_->dimDescs_.getChunkContainerDims().data(), 
						 &this->chunks_,
						 itMode);
}
arrayBase::size_type arrayBase::getNumChunks()
{
	return this->chunks_.size();
}
coor arrayBase::itemCoorToChunkCoor(coor& itemCoor)
{
	coor chunkCoor(this->desc_->dimDescs_.size());
	for(dimensionId d = 0; d < this->desc_->dimDescs_.size(); d++)
	{
		chunkCoor[d] = floor(itemCoor[d] / this->desc_->dimDescs_[d]->chunkSize_);
	}
	return chunkCoor;
}
void arrayBase::insertChunk(pChunk inputChunk)
{
	this->chunks_.insert(chunkPair(inputChunk->getId(), inputChunk));
}

pChunk arrayBase::getChunk(chunkId cId)
{
	return this->chunks_[cId];
}

arrayId arrayBase::getArrayId()
{
	return this->desc_->id_;
}

chunkId arrayBase::getChunkId(pChunkDesc cDesc)
{
	return this->getChunkIdFromItemCoor(cDesc->sp_);
}

chunkId arrayBase::getChunkIdFromItemCoor(coor& itemCoor)
{
	coor chunkCoor = itemCoor;
	for (dimensionId d = this->desc_->dimDescs_.size() - 1; d != -1; d--)
	{
		chunkCoor[d] /= this->desc_->dimDescs_[d]->chunkSize_;
	}
	return this->getChunkIdFromChunkCoor(chunkCoor);
}

chunkId arrayBase::getChunkIdFromChunkCoor(coor& chunkCoor)
{
	chunkId id = 0;
	chunkId offset = 1;
	for(dimensionId d = this->desc_->dimDescs_.size() - 1; d != -1; d--)
	{
		id += offset * chunkCoor[d];
		offset *= this->desc_->dimDescs_[d]->getChunkNum();
	}
	return id;
}

chunkIterator::chunkIterator(const size_type dSize, dim_const_pointer dims, 
							 chunkIterator::chunkContainer* chunks, iterateMode itMode)
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
}