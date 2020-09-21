#include <op/spiht_decode/spiht_decode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <array/memChunk.h>
#include <compression/spihtChunk.h>
#include <compression/wtChunk.h>

namespace msdb
{
spiht_decode_action::spiht_decode_action()
{
}

spiht_decode_action::~spiht_decode_action()
{
}

const char* spiht_decode_action::name()
{
	return "spiht_decode_action";
}

pArray spiht_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);

	auto planBitmap = this->getPlanChunkBitmap();
	auto arrDesc = this->getArrayDesc();
	dimension originalChunkDims = arrDesc->getDimDescs()->getChunkDims();
	for (dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()->at(d)->chunkSize_ = arrDesc->getDimDescs()->at(d)->blockSize_;
	}

	auto outArr = std::make_shared<wavelet_encode_array>(this->getArrayDesc());
	outArr->setMaxLevel(maxLevel);
	outArr->setOrigianlChunkDims(originalChunkDims);
	outArr->copyChunkBitmap(planBitmap);

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		this->decodeAttribute(std::static_pointer_cast<wavelet_encode_array>(outArr), attr, qry);
	}
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void spiht_decode_action::decodeAttribute(std::shared_ptr<wavelet_encode_array> outArr, 
										  pAttributeDesc attrDesc, pQuery qry)
{
	auto cit = outArr->getChunkIterator(iterateMode::ALL);

	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();
						// TODO:: Use to makeChunk function
						//outArr->makeChunk(attr->id_, cId);
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto blockBitmap = this->getPlanBlockBitmap(cid);
			if (blockBitmap)
			{
				inChunk->copyBlockBitmap(blockBitmap);
			} else
			{
				// If there were no bitmap, set all blocks as true.
				inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
			}
			inChunk->makeAllBlocks();
			inChunk->setMaxLevel(outArr->getMaxLevel());

			//========================================//
			qry->getTimer()->nextWork(0, workType::IO);
			//----------------------------------------//
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(inChunk);
			storageMgr::instance()->loadChunk(outArr->getId(), attrDesc->id_, cid, serialChunk);

			//========================================//
			qry->getTimer()->nextWork(0, workType::COMPUTING);
			//----------------------------------------//

			auto outChunk = outArr->makeChunk(*inChunk->getDesc());
			auto wtOutChunk = std::static_pointer_cast<wtChunk>(outChunk);
			wtOutChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
			wtOutChunk->makeAllBlocks();
			outChunk->bufferCopy(inChunk);	// 여기 왜 copy지? 왜ref 하면 애러나지?
		}

		++(*cit);
	}
}
pSpihtChunk spiht_decode_action::makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc, chunkId cid)
{
	return std::make_shared<spihtChunk>(arr->getChunkDesc(attrDesc->id_, cid));
}
}