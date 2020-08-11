#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/haar.h>
#include <util/math.h>
#include <list>
#include <memory>

namespace msdb
{
msdb::wavelet_encode_action::wavelet_encode_action()
{
	this->waveletName_ = "HaarSimple";
}

wavelet_encode_action::~wavelet_encode_action()
{
}

pArray wavelet_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray inArr = inputArrays[0];

	//////////////////////////////
	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	//////////////////////////////
	// Build wavelet_encode_array
	auto outArr = std::make_shared<wavelet_encode_array>(this->getArrayDesc(), maxLevel);
	// maxLevel value is checked in wavelet_encode_array constructor
	// which can be used for current array.
	maxLevel = outArr->getMaxLevel();

	for(auto attrDesc : inArr->getDesc()->getAttrDescs())
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			attributeEncode<char>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::INT8:
			attributeEncode<int8_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::INT16:
			attributeEncode<int16_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::INT32:
			attributeEncode<int32_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::INT64:
			attributeEncode<int64_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::UINT8:
			attributeEncode<uint8_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::UINT16:
			attributeEncode<uint16_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::UINT32:
			attributeEncode<uint32_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		case eleType::UINT64:
			attributeEncode<uint64_t>(outArr, inArr, attrDesc, w, maxLevel, q);
			break;
		//case eleType::DOUBLE:
		//	attributeEncode<double>(inChunk, arrRange,w, d, q);
		//	break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	return outArr;
}
//
//std::list<pChunk> wavelet_encode_action::chunkEncode(pArray wArray, pChunk sourceChunk,
//													 pWavelet w, size_t maxLevel, pQuery q)
//{
//	std::cout << "chunkEncode" << std::endl;
//	std::list<pChunk> chunks;
//	size_t blockCnt = wArray->getDesc()->getDimDescs().getBlockDims().area();
//	auto bItr = sourceChunk->getBlockIterator();
//
//	while (!bItr->isEnd())
//	{
//		auto wtc = this->blockEncode(wArray, sourceChunk, bItr->seqPos(), w, maxLevel, q);
//		wtc->setId(sourceChunk->getId() * blockCnt + bItr->seqPos());
//		auto castedWtc = std::static_pointer_cast<wtChunk>(wtc);	// cast to wtChunk
//		castedWtc->setSourceChunkId(sourceChunk->getId());
//		castedWtc->setLevel(maxLevel);
//		chunks.push_back(wtc);
//		++(*bItr);
//
//		std::cout << "====================" << std::endl;
//		std::cout << "chunkEncode::" << std::endl;
//		std::cout << "CID: " << sourceChunk->getId() << " / BID: " << (*bItr)->getId() << std::endl;
//		castedWtc->print();
//		std::cout << "====================" << std::endl;
//	}
//
//	return chunks;
//}
//
//pChunk wavelet_encode_action::blockEncode(pArray wArray, pChunk inChunk,
//										  blockId bid, pWavelet w, size_t maxLevel, pQuery q)
//{
//	auto iBlock = inChunk->getBlock(bid);
//	auto inChunkDesc = inChunk->getDesc();
//	auto inBlockDesc = iBlock->getDesc();
//	auto blockCoor = inBlockDesc->blockCoor_;
//	auto blockSpace = inChunkDesc->getBlockSpace();
//
//	// Setup outChunkDesc
//	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunkDesc);
//	outChunkDesc->dims_ = outChunkDesc->blockDims_;
//	outChunkDesc->chunkCoor_ = inChunkDesc->chunkCoor_ * blockSpace + blockCoor;
//	outChunkDesc->sp_ = inChunkDesc->sp_ + inBlockDesc->sp_;
//	outChunkDesc->ep_ = inChunkDesc->ep_ + inBlockDesc->ep_;
//	outChunkDesc->mSize_ = inBlockDesc->mSize_;
//	// TODO:: fill cSize
//	//outChunkDesc->cSize_	
//
//	pChunk outChunk = std::make_shared<wtChunk>(outChunkDesc);
//	outChunk->bufferCopy(iBlock);
//	//std::vector<coorRange> ranges;
//	//ranges.push_back(coorRange(outChunkDesc->blockDims_));
//
//	for (size_t level = 0; level <= maxLevel; level++)
//	{
//		coorRange arrRange = outChunkDesc->blockDims_ / pow(2, level);
//		outChunk = this->waveletLevelEncode(outChunk, arrRange, w, q);
//		//chunks.pop_front();
//		//chunkId bandId = 0;
//		//for(auto c : l)
//		//{
//		//	pWtChunk wtChunk_ = std::static_pointer_cast<wtChunk>(c);
//		//	wtChunk_->setBandId(bandId);
//		//	wtChunk_->setLevel(level);
//		//	wtChunk_->setSourceChunkId(sourceChunkId);
//		//	++bandId;
//		//}
//	}
//	return outChunk;
//}
//
//pChunk wavelet_encode_action::waveletLevelEncode(pChunk inChunk, coorRange arrRange, pWavelet w, pQuery q)
//{
//	switch (inChunk->getDesc()->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//		return waveletTransform<char>(inChunk, arrRange, w, q);
//		break;
//	case eleType::INT8:
//		return waveletTransform<int8_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::INT16:
//		return waveletTransform<int16_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::INT32:
//		return waveletTransform<int32_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::INT64:
//		return waveletTransform<int64_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::UINT8:
//		return waveletTransform<uint8_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::UINT16:
//		return waveletTransform<uint16_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::UINT32:
//		return waveletTransform<uint32_t>(inChunk, arrRange, w, q);
//		break;
//	case eleType::UINT64:
//		return waveletTransform<uint64_t>(inChunk, arrRange, w, q);
//		break;
//	//case eleType::DOUBLE:
//	//	waveletTransform<double>(inChunk, arrRange,w, d, q);
//	//	break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//}

const char* msdb::wavelet_encode_action::name()
{
	return "wavelet_encode";
}
}

