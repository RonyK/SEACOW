#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/memChunk.h>
#include <compression/spihtChunk.h>

namespace msdb
{
spiht_encode_action::spiht_encode_action()
{
}

spiht_encode_action::~spiht_encode_action()
{
}

const char* spiht_encode_action::name()
{
	return "spiht_encode_action";
}

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray sourceArr = inputArrays[0];
	auto wArray = std::static_pointer_cast<wavelet_encode_array>(sourceArr);
	arrayId arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
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
}