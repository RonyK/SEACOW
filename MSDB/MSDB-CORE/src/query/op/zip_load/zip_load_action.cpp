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

	// Get Parameter - NumBin
	eleDefault numBins;
	std::static_pointer_cast<stableElement>(this->params_[1]->getParam())->getData(&numBins);

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		this->loadAttribute(outArr, attr, numBins, qry);
	}

	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}
void zip_load_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, eleDefault numBins, pQuery qry)
{
	auto cit = outArr->getChunkIterator(iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();

			auto inChunk = this->makeInChunk(outArr, attrDesc, cid, numBins);


			//========================================//
			qry->getTimer()->nextWork(0, workType::IO);
			//----------------------------------------//
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(inChunk);
			storageMgr::instance()->loadChunk(outArr->getId(), attrDesc->id_, (inChunk)->getId(),
											  serialChunk);

			//========================================//
			qry->getTimer()->nextWork(0, workType::COMPUTING);
			//----------------------------------------//
			auto outChunk = outArr->makeChunk(*inChunk->getDesc());
			outChunk->bufferCopy(inChunk);
			outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
			outChunk->makeAllBlocks();
		}

		++(*cit);
	}
}
pZipChunk zip_load_action::makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid, eleDefault numBins)
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
	inChunk->makeBlocks(*inChunk->getBlockBitmap());

	return inChunk;
}
}	// msdb