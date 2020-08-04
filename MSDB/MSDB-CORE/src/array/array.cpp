#include <array/array.h>
#include <util/math.h>
#include <cmath>

namespace msdb
{
arrayBase::arrayBase(pArrayDesc desc)
{
	this->desc_ = desc;
}
arrayBase::~arrayBase()
{
	std::cout << "~arrayBase()" << std::endl;
}
pArrayDesc arrayBase::getDesc()
{
	return this->desc_;
}
pChunkIterator arrayBase::getChunkIterator(iterateMode itMode)
{
	return std::make_shared<chunkIterator>(this->desc_->dimDescs_.size(), 
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

void arrayBase::flush()
{
	this->chunks_.clear();
}

pChunk arrayBase::getChunk(chunkId cId)
{
	return this->chunks_[cId];
}

arrayId arrayBase::getId()
{
	return this->desc_->id_;
}

void arrayBase::setId(arrayId id)
{
	this->desc_->id_ = id;
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
void arrayBase::print()
{
	auto cit = this->getChunkIterator();

	while(!cit->isEnd())
	{
		(**cit)->print();
		++(*cit);
	}
}
}
