#include <op/se_decompression/se_decompression_action.h>
#include <array/memArray.h>
#include <system/storageMgr.h>

namespace msdb
{
se_decompression_action::se_decompression_action()
{
}
se_decompression_action::~se_decompression_action()
{
}
const char* se_decompression_action::name()
{
	return "se_decompression_action";
}
pArray se_decompression_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	auto arrDesc = this->getArrayDesc();
	dimension origianlChunkDims = arrDesc->getDimDescs().getChunkDims();
	for(dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()[d]->chunkSize_ = arrDesc->getDimDescs()[d]->blockSize_;
	}

	auto outArr = std::make_shared<wavelet_encode_array>(arrDesc);
	outArr->setMaxLevel(0);
	outArr->setOrigianlChunkDims(origianlChunkDims);
	auto arrId = outArr->getId();

	for (auto attrDesc : outArr->getDesc()->attrDescs_)
	{
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		while(!cit->isEnd())
		{
			// make chunk
			chunkId cid = cit->seqPos();
			coor chunkCoor = cit->coor();
			auto outChunk = this->makeOutChunk(outArr, attrDesc, cid, chunkCoor);
			outChunk->bufferAlloc();
			outChunk->makeAllBlocks();

			outArr->insertChunk(outChunk);
			
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->loadChunk(arrId, attrDesc->id_, (**cit)->getId(),
											  serialChunk);
			++(*cit);
		}
	}

	return std::static_pointer_cast<arrayBase>(outArr);;
}

pWtChunk se_decompression_action::makeOutChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
											   chunkId cid, coor chunkCoor)
{
	dimension chunkDims = arr->getDesc()->getDimDescs().getChunkDims();
	dimension blockDims = arr->getDesc()->getDimDescs().getBlockDims();

	coor sp = chunkDims * chunkCoor;
	coor ep = sp + chunkDims;

	chunkSize mSize = chunkDims.area() * attrDesc->typeSize_;

	dimension originalSourceDims = arr->getOrigianlChunkDims();

	auto outDesc = std::make_shared<chunkDesc>(cid, attrDesc, 
											   chunkDims, blockDims, 
											   sp, ep, mSize);
	pWtChunk outChunk = std::make_shared<wtChunk>(outDesc);
	outChunk->setLevel(arr->getMaxLevel());
	//outChunk->setSourceChunkId(sourceChunkId);
	outChunk->bufferAlloc();
	outChunk->makeAllBlocks();

	return outChunk;
}
}	// msdb