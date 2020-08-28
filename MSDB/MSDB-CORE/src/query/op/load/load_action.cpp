#include <op/load/load_action.h>
#include <array/memBlockArray.h>
#include <array/blockChunk.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>

namespace msdb
{
load_action::load_action()
{
}
load_action::~load_action()
{
}
const char* load_action::name()
{
	return "load_action";
}
pArray load_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);
	auto planChunkBitmap = this->getPlanChunkBitmap();

	pArray outArr = arrayMgr::instance()->makeArray<memBlockArray>(this->getArrayDesc());
	outArr->copyChunkBitmap(planChunkBitmap);
	arrayId arrId = outArr->getId();

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		while (!cit->isEnd())
		{
			if(cit->isExist())
			{
				chunkId cId = cit->seqPos();
				auto outChunk = outArr->makeChunk(attr->id_, cId);

				auto blockBitmap = this->getPlanBlockBitmap(cId);
				if(blockBitmap)
				{
					outChunk->copyBlockBitmap(blockBitmap);
				}else
				{
					// If there were no bitmap, set all blocks as true.
					outChunk->replaceBlockBitmap(std::make_shared<bitmap>(outChunk->getBlockCapacity(), true));
				}
				outChunk->makeAllBlocks();
				//outArr->insertChunk(std::make_shared<memBlockChunk>(outArr->getChunkDesc(attr->id_, cId)));

				pSerializable serialChunk
					= std::static_pointer_cast<serializable>(**cit);
				storageMgr::instance()->loadChunk(arrId, attr->id_, (**cit)->getId(),
												  serialChunk);
			}
			
			++(*cit);
		}
	}

	return outArr;
}
}	// msdb