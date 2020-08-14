#include <op/load/load_action.h>
#include <array/memArray.h>
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

	pArray outArr = arrayMgr::instance()->makeArray<arrayBase>(this->getArrayDesc());
	arrayId arrId = outArr->getId();

	for (auto attr : outArr->getDesc()->attrDescs_)
	{
		auto cit = outArr->getChunkIterator(iterateMode::ALL);

		while (!cit->isEnd())
		{
			chunkId cId = cit->seqPos();
			outArr->insertChunk(std::make_shared<memBlockChunk>(outArr->getChunkDesc(cId, attr->id_)));

			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->loadChunk(arrId, attr->id_, (**cit)->getId(),
											  serialChunk);
			++(*cit);
		}
	}

	return outArr;
}
}	// msdb