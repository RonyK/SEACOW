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
	auto planBitmap = this->getPlanChunkBitmap();

	auto arrDesc = this->getArrayDesc();
	dimension originalChunkDims = arrDesc->getDimDescs()->getChunkDims();
	for (dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()->at(d)->chunkSize_ = arrDesc->getDimDescs()->at(d)->blockSize_;
	}

	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);

	auto outArr = std::make_shared<wavelet_encode_array>(arrDesc);
	outArr->setMaxLevel(maxLevel);
	outArr->setOrigianlChunkDims(originalChunkDims);
	outArr->copyChunkBitmap(planBitmap);
	auto arrId = outArr->getId();

	for (auto attrDesc : *outArr->getDesc()->attrDescs_)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			decompressAttribute<char>(outArr, attrDesc);
			break;
		case eleType::INT8:
			decompressAttribute<int8_t>(outArr, attrDesc);
			break;
		case eleType::INT16:
			decompressAttribute<int16_t>(outArr, attrDesc);
			break;
		case eleType::INT32:
			decompressAttribute<int32_t>(outArr, attrDesc);
			break;
		case eleType::INT64:
			decompressAttribute<int64_t>(outArr, attrDesc);
			break;
		case eleType::UINT8:
			decompressAttribute<uint8_t>(outArr, attrDesc);
			break;
		case eleType::UINT16:
			decompressAttribute<uint16_t>(outArr, attrDesc);
			break;
		case eleType::UINT32:
			decompressAttribute<uint32_t>(outArr, attrDesc);
			break;
		case eleType::UINT64:
			decompressAttribute<uint64_t>(outArr, attrDesc);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	return std::static_pointer_cast<arrayBase>(outArr);;
}

pSeChunk se_decompression_action::makeInChunk(std::shared_ptr<wavelet_encode_array> arr,
											  pAttributeDesc attrDesc,
											  chunkId cid, coor chunkCoor)
{
	dimension chunkDims = arr->getDesc()->getDimDescs()->getChunkDims();
	dimension blockDims = arr->getDesc()->getDimDescs()->getBlockDims();

	coor sp = chunkDims * chunkCoor;
	coor ep = sp + chunkDims;

	chunkSize mSize = chunkDims.area() * attrDesc->typeSize_;

	dimension originalSourceDims = arr->getOrigianlChunkDims();

	auto outDesc = std::make_shared<chunkDesc>(cid, std::make_shared<attributeDesc>(*attrDesc),
											   chunkDims, blockDims,
											   sp, ep, mSize);
	pSeChunk outChunk = std::make_shared<seChunk>(outDesc);
	outChunk->setLevel(arr->getMaxLevel());
	//inChunk->setSourceChunkId(sourceChunkId);
	outChunk->bufferAlloc();
	outChunk->makeAllBlocks();

	return outChunk;
}
}	// msdb