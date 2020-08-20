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

	for (auto attr : outArr->getDesc()->attrDescs_)
	{
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		while (!cit->isEnd())
		{
			chunkId cId = cit->seqPos();
			outArr->insertChunk(std::make_shared<spihtChunk>(outArr->getChunkDesc(cId, attr->id_)));

			auto spChunk = std::static_pointer_cast<spihtChunk>(**cit);
			spChunk->setMaxLevel(maxLevel);
			spChunk->makeAllBlocks();
			spChunk->bufferAlloc();
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->loadChunk(arrId, attr->id_, (**cit)->getId(),
				serialChunk);
			++(*cit);
		}
	}

	return outArr;

	//pArray sourceArr = inputArrays[0];
	//auto wArray = std::static_pointer_cast<wavelet_encode_array>(sourceArr);
	//arrayId arrId = sourceArr->getId();

	//auto cc1 = wArray->getChunkIterator();
	//auto bb1 = (**cc1)->getBlockIterator();
	//auto aa1 = (**bb1)->getItemIterator();
	//coor child_coor1(2);
	//child_coor1[0] = 1;
	//child_coor1[1] = 0;
	//aa1->moveTo(child_coor1);
	//auto b1 = (**aa1).get<char>();

	//for (auto attr : sourceArr->getDesc()->attrDescs_)
	//{
	//	auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);
	//	while (!cit->isEnd())
	//	{
	//		pChunk inChunk = (**cit);
	//		auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
	//		pSpihtChunk outChunk = std::make_shared<spihtChunk>(outChunkDesc);
	//		outChunk->setMaxLevel(wArray->getMaxLevel());
	//		outChunk->makeAllBlocks();
	//		outChunk->bufferRef(inChunk);

	//		pSerializable serialChunk
	//			= std::static_pointer_cast<serializable>(outChunk);
	//		storageMgr::instance()->loadChunk(arrId, attr->id_, (outChunk)->getId(),
	//			serialChunk);

	//		++(*cit);
	//	}
	//}
	
	//auto cc = wArray->getChunkIterator();
	//auto bb = (**cc)->getBlockIterator();
	//auto aa = (**bb)->getItemIterator();
	//coor child_coor(2);
	//child_coor[0] = 1;
	//child_coor[1] = 0;
	//aa->moveTo(child_coor);
	//auto b = (**aa).get<char>();

	//return sourceArr;	
}
}