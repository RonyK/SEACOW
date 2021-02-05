#include <stdafx.h>
#include <op/zip_load/zip_load_action.h>
#include <array/memBlockArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
zip_load_action::zip_load_action()
{
}
zip_load_action::~zip_load_action()
{
}
const char* zip_load_action::name()
{
	return "zip_load_action";
}
pArray zip_load_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	auto outArr = std::make_shared<memBlockArray>(this->getArrayDesc());
	outArr->copyChunkBitmap(this->getPlanChunkBitmap());

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		this->loadAttribute(outArr, attr, qry);
	}

	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}
void zip_load_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	auto cit = outArr->getChunkIterator(iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(*inChunk->getDesc());

			this->loadChunk(outArr, outChunk, inChunk, attrDesc->id_, qry, 0);
		}

		++(*cit);
	}
}

void zip_load_action::loadChunk(pArray outArr, pChunk outChunk, pZipChunk inChunk, attributeId attrId, pQuery qry, const size_t parentThreadId)
{
	//========================================//
	qry->getTimer()->nextWork(0, workType::IO);
	//----------------------------------------//
	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(inChunk);
	storageMgr::instance()->loadChunk(outArr->getId(), attrId, (inChunk)->getId(),
									  serialChunk);

	//========================================//
	qry->getTimer()->nextWork(0, workType::COMPUTING);
	//----------------------------------------//

	outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferCopy(inChunk);
}

pZipChunk zip_load_action::makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid)
{
	auto inChunkDesc = std::make_shared<chunkDesc>(*inArr->getChunkDesc(attrDesc->id_, cid));
	auto inChunk = std::make_shared<zipChunk>(inChunkDesc);
	auto blockBitmap = this->getPlanBlockBitmap(cid);
	if (blockBitmap)
	{
		inChunk->copyBlockBitmap(blockBitmap);
	} else
	{
		// If there were no bitmap, set all blocks as true.
		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
	}
	inChunk->makeBlocks();

	return inChunk;
}
}	// msdb