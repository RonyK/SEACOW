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

	auto cc1 = wArray->getChunkIterator();
	auto bb1 = (**cc1)->getBlockIterator();
	auto aa1 = (**bb1)->getItemIterator();
	coor child_coor1(2);
	child_coor1[0] = 1;
	child_coor1[1] = 0;
	aa1->moveTo(child_coor1);
	auto b1 = (**aa1).get<char>();

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

	auto cc = wArray->getChunkIterator();
	auto bb = (**cc)->getBlockIterator();
	auto aa = (**bb)->getItemIterator();
	coor child_coor(2);
	child_coor[0] = 1;
	child_coor[1] = 0;
	aa->moveTo(child_coor);
	auto b = (**aa).get<char>();

	return sourceArr;
}
}