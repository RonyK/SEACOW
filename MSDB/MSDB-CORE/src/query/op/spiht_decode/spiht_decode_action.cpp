#include <op/spiht_decode/spiht_decode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <array/memChunk.h>
#include <compression/spihtChunk.h>

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

pArray spiht_decode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	auto maxLevel = std::static_pointer_cast<wavelet_encode_array>(inputArrays[0])->getMaxLevel();
	auto originalChunkDims = std::static_pointer_cast<wavelet_encode_array>(inputArrays[0])->getOrigianlChunkDims();
	pArray outArr = arrayMgr::instance()->makeArray<wavelet_encode_array>(this->getArrayDesc());
	arrayId arrId = outArr->getId();


	std::static_pointer_cast<wavelet_encode_array>(outArr)->setMaxLevel(maxLevel);
	std::static_pointer_cast<wavelet_encode_array>(outArr)->setOrigianlChunkDims(originalChunkDims);

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		while (!cit->isEnd())
		{
			chunkId cId = cit->seqPos();
			// TODO:: Use to makeChunk function
			outArr->insertChunk(attr->id_, std::make_shared<spihtChunk>(outArr->getChunkDesc(attr->id_, cId)));

			auto spChunk = std::static_pointer_cast<spihtChunk>(**cit);
			spChunk->setMaxLevel(maxLevel);
			spChunk->makeAllBlocks();
			spChunk->bufferAlloc();
			// spChunk->initBufferZero();
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->loadChunk(arrId, attr->id_, (**cit)->getId(),
				serialChunk);
			++(*cit);
		}
	}

	return outArr;
}
}