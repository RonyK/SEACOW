#include <op/save/save_action.h>
#include <array/memArray.h>
#include <system/storageMgr.h>

namespace msdb
{
save_action::save_action()
{
}
save_action::~save_action()
{
}
const char* save_action::name()
{
	return "save_action";
}
pArray save_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getArrayId();

	for (auto attr : sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);

		while (!cit.isEnd())
		{
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(*cit);
			storageMgr::instance()->saveChunk(arrId, attr->id_, (*cit)->getId(),
											  serialChunk);
			++cit;
		}
	}

	return sourceArr;
}
}	// msdb