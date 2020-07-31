#include <op/load/load_action.h>
#include <array/memArray.h>
#include <system/storageMgr.h>

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

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	for (auto attr : sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::ALL);

		while (!cit.isEnd())
		{
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(*cit);
			storageMgr::instance()->loadChunk(arrId, attr->id_, (*cit)->getId(),
											  serialChunk);
			++cit;
		}
	}

	return sourceArr;
}
}	// msdb