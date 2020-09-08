#include <op/between/between_action.h>
#include <array/arrayMgr.h>
#include <array/blockChunk.h>
#include <array/memBlockArray.h>

namespace msdb
{
between_action::between_action()
{
}

between_action::~between_action()
{
}

const char* between_action::name()
{
	return "between_action";
}

pArray between_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray inArr = inputArrays[0];
	pArray outArr = arrayMgr::instance()->makeArray<memBlockArray>(this->getArrayDesc());
	pCoor sp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	pCoor ep = std::static_pointer_cast<coor>(this->params_[2]->getParam());
	coorRange betweenRange(*sp, *ep);

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		auto chunkItr = inArr->getChunkIterator();
		while (!chunkItr->isEnd())
		{
			auto inChunk = (**chunkItr);
			auto chunkRange = inChunk->getChunkRange();

			if (chunkRange.isIntersect(betweenRange))
			{
				auto outChunk = outArr->makeChunk(attr->id_, inChunk->getId());
				outChunk->bufferRef(inChunk);

				if (chunkRange.isFullyInside(betweenRange))
				{
					this->fullyInsideChunk(outChunk, inChunk);
				}else
				{
					this->betweenChunk(outChunk, inChunk, betweenRange);
				}
			}
			++(*chunkItr);
		}
	}
	
	return outArr;
}
void between_action::betweenChunk(pChunk outChunk, pChunk inChunk, coorRange& betweenRange)
{
	auto blockItr = inChunk->getBlockIterator();
	auto betweenRangeInChunk = betweenRange;
	betweenRangeInChunk.move(inChunk->getDesc()->sp_ * -1);

	while (!blockItr->isEnd())
	{
		auto inBlock = (**blockItr);
		auto blockRange = inBlock->getBlockRange();

		if(blockRange.isIntersect(betweenRangeInChunk))
		{
			auto outBlock = outChunk->makeBlock(inBlock->getId());

			if(blockRange.isFullyInside(betweenRangeInChunk))
			{
				fullyInsideBlock(outBlock, inBlock);
			}else
			{
				this->betweenBlock(outBlock, inBlock, betweenRangeInChunk);
			}
		}

		++(*blockItr);
	}
}
void between_action::fullyInsideChunk(pChunk outChunk, pChunk inChunk)
{
	outChunk->makeBlocks(*inChunk->getBlockBitmap());
}
void between_action::betweenBlock(pBlock outBlock, pBlock inBlock, coorRange& betweenRangeInChunk)
{
	auto inDesc = inBlock->getDesc();
	auto outDesc = outBlock->getDesc();

	outDesc->isp_ = getOutsideCoor(inDesc->isp_, betweenRangeInChunk.getEp());
	outDesc->iep_ = getInsideCoor(inDesc->iep_, betweenRangeInChunk.getEp());
	//outDesc->mSize_ // recalculate
}
void between_action::fullyInsideBlock(pBlock outBlock, pBlock inBlock)
{
	auto inDesc = inBlock->getDesc();
	auto outDesc = outBlock->getDesc();

	outDesc->isp_ = inDesc->isp_;
	outDesc->iep_ = inDesc->iep_;
	outDesc->mSize_ = inDesc->mSize_;
}
}