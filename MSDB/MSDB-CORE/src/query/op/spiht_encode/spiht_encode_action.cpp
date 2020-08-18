#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/memChunk.h>
#include <compression/spihtChunk.h>

namespace msdb
{
spiht_encode_action::spiht_encode_action()
{
	//codeBs_ << setw(1);
}

spiht_encode_action::~spiht_encode_action()
{
}

const char* spiht_encode_action::name()
{
	return "spiht_encode";
}

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray sourceArr = inputArrays[0];
	auto wArray = std::static_pointer_cast<wavelet_encode_array>(sourceArr);
	arrayId arrId = sourceArr->getId();

	for (auto attr : sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);
		while (!cit->isEnd())
		{
			pChunk inChunk = (**cit);
			auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
			pSpihtChunk outChunk = std::make_shared<spihtChunk>(outChunkDesc);
			outChunk->setMaxLevel(wArray->getMaxLevel());
			outChunk->makeAllBlocks();
			outChunk->bufferRef(inChunk);

			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(outChunk);
			storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
											  serialChunk);
			++(*cit);
		}
	}

	return sourceArr;
}

//pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
//{
//	auto source = inputArrays[0];
//	auto arrId = source->getId();
//	auto wArray = std::static_pointer_cast<wavelet_encode_array>(source);
//	auto chunkItr = wArray->getChunkIterator();
//	while (!chunkItr->isEnd())
//	{
//		auto blockItr = (**chunkItr)->getBlockIterator();
//		while (!blockItr->isEnd())
//		{
//			auto itemItr = (**blockItr)->getItemIterator();
//			auto blockDims = wArray->getDesc()->dimDescs_.getBlockDims();		// chunk size
//			auto dSize = blockDims.size();								// dimension size
//			auto max_level = wArray->getMaxLevel();							// max level
//			std::vector<size_t> bandSize(dSize);							// band size in max level(?)
//			for (int d = (int)dSize - 1; d >= 0; d--)
//			{
//				bandSize[d] = (size_t)(blockDims[d] / pow(2, max_level + 1));
//			}
//
//			this->encode_init(dSize, bandSize);
//
//			switch ((**chunkItr)->getDesc()->attrDesc_->type_)
//			{
//			case eleType::CHAR:
//				this->encode_progress<char>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::INT8:
//				this->encode_progress<int8_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::INT16:
//				this->encode_progress<int16_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::INT32:
//				this->encode_progress<int32_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::INT64:
//				this->encode_progress<int64_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::UINT8:
//				this->encode_progress<uint8_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::UINT16:
//				this->encode_progress<uint16_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::UINT32:
//				this->encode_progress<uint32_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			case eleType::UINT64:
//				this->encode_progress<uint64_t>(dSize, blockDims, bandSize, itemItr);
//				break;
//			default:
//				_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//			}
//
//			/*
//			auto attr = (**chunkItr)->getDesc()->attrDesc_;
//			pChunk oChunk = std::make_shared<memChunk>((**chunkItr)->getDesc());
//			oChunk->bufferRef(this->codeBs_.data(), this->codeBs_.capacity());
//			storageMgr::instance()->saveChunk(arrId, attr->id_, (**chunkItr)->getId(), std::static_pointer_cast<serializable>(oChunk));
//			*/
//
//			this->codeBs_.flush();
//
//			++(*blockItr);
//		}
//		++(*chunkItr);
//	}
//
//	return source;
//}
//
//void spiht_encode_action::encode_init(size_t dSize, std::vector<size_t> bandSize)
//{
//	this->LIP_.clear();
//	this->LIS_.clear();
//	this->LIS_TYPE_.clear();
//	this->LSP_.clear();
//	
//	coor init_coor(dSize);	// {0, 0, ...}
//	size_t init_num = 1;
//	for (int d = (int)dSize - 1; d >= 0; d--)
//	{
//		init_coor[d] = 0;
//		init_num *= bandSize[d];
//	}
//	
//	for (size_t i = 0; i < init_num; i++)
//	{
//		this->LIP_.push_back(init_coor);
//
//		bool haveChild = false;
//		for (int d = (int)dSize - 1; d >= 0; d--)
//		{
//			if (init_coor[d] & 0x1)		// odd coordinate
//			{
//				haveChild = true;
//				break;
//			}
//		}
//
//		if (haveChild)
//		{
//			this->LIS_.push_back(init_coor);
//			this->LIS_TYPE_.push_back('A');
//		}
//
//
//		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
//		{
//			init_coor[d] = init_coor[d] + 1;
//			if (init_coor[d] == bandSize[d])
//			{
//				init_coor[d] = 0;
//			}
//			else
//			{
//				break;
//			}
//		}
//	}
//}
}