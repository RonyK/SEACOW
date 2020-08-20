#include <array/array.h>
#include <util/math.h>
#include <cmath>

namespace msdb
{
arrayBase::arrayBase(pArrayDesc desc)
	: chunkBitmap_()
{
	this->desc_ = desc;
	this->chunkBitmap_.resize(desc->attrDescs_->size(), bitmap(desc_->dimDescs_->getChunkSpace()));
}
arrayBase::~arrayBase()
{
	//std::cout << "~arrayBase()" << std::endl;
}
pArrayDesc arrayBase::getDesc()
{
	return this->desc_;
}
pChunkIterator arrayBase::getChunkIterator(iterateMode itMode)
{
	return std::make_shared<chunkIterator>(this->desc_->dimDescs_->size(),
										   this->desc_->dimDescs_->getChunkSpace().data(),
										   &this->chunks_,
										   itMode);
}
arrayBase::size_type arrayBase::getNumChunks()
{
	return this->chunks_.size();
}
coor arrayBase::itemCoorToChunkCoor(const coor& itemCoor)
{
	coor chunkCoor(this->desc_->dimDescs_->size());
	for (dimensionId d = 0; d < this->desc_->dimDescs_->size(); d++)
	{
		chunkCoor[d] = floor(itemCoor[d] / (*this->desc_->dimDescs_)[d]->chunkSize_);
	}
	return chunkCoor;
}
void arrayBase::insertChunk(const attributeId attrId, pChunk inputChunk)
{
	assert(attrId < this->desc_->attrDescs_->size());
	this->chunks_.insert(chunkPair(inputChunk->getId(), inputChunk));
	this->chunkBitmap_[attrId].setExist(inputChunk->getId());
}

void arrayBase::flush()
{
	this->chunks_.clear();
}

pChunk arrayBase::makeChunk(const chunkDesc& desc)
{
	return this->makeChunk(desc.attrDesc_->id_, desc.id_);
}

void arrayBase::setChunkBitmap(const attributeId attrId, const bitmap& input)
{
	auto cit = this->getChunkIterator();
	while(!cit->isEnd())
	{
		if(input.isExist(cit->seqPos()) && (**cit) == nullptr)
		{
			this->makeChunk(attrId, cit->seqPos());
		}
	}
}

pChunkDesc arrayBase::getChunkDesc(const attributeId attrId, const chunkId cId)
{
	dimension chunkDims = this->desc_->getDimDescs()->getChunkDims();
	dimension blockDims = this->desc_->getDimDescs()->getBlockDims();
	pAttributeDesc attrDesc = (*this->desc_->getAttrDescs())[attrId];
	auto cItr = this->getChunkIterator();
	coor chunkCoor = cItr->seqToCoor(cId);
	dimension sp = chunkDims * chunkCoor;
	dimension ep = sp + chunkDims;

	return std::make_shared<chunkDesc>(cId,
									   attrDesc,
									   chunkDims, blockDims,
									   sp, ep,
									   chunkDims.area() * attrDesc->typeSize_);
}

pChunk arrayBase::getChunk(const chunkId cId)
{
	return this->chunks_[cId];
}

arrayId arrayBase::getId()
{
	return this->desc_->id_;
}

void arrayBase::setId(const arrayId id)
{
	this->desc_->id_ = id;
}

chunkId arrayBase::getChunkId(pChunkDesc cDesc)
{
	return this->getChunkIdFromItemCoor(cDesc->sp_);
}

chunkId arrayBase::getChunkIdFromItemCoor(const coor& itemCoor)
{
	coor chunkCoor = itemCoor;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; d--)
	{
		chunkCoor[d] /= this->desc_->dimDescs_->at(d)->chunkSize_;
	}
	return this->getChunkIdFromChunkCoor(chunkCoor);
}

chunkId arrayBase::getChunkIdFromChunkCoor(const coor& chunkCoor)
{
	chunkId id = 0;
	chunkId offset = 1;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; d--)
	{
		id += offset * chunkCoor[d];
		offset *= (*this->desc_->dimDescs_)[d]->getChunkNum();
	}
	return id;
}
void arrayBase::print()
{
	auto cit = this->getChunkIterator();

	while (!cit->isEnd())
	{
		(**cit)->print();
		++(*cit);
	}
}
}
