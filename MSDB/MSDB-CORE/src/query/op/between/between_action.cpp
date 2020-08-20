#include <op/between/between_action.h>

namespace msdb
{
between_action::between_action()
{
}

between_action::~between_action()
{
}

const char* between_action::name()
{
	return "between_action";
}

pArray between_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray inputArray = inputArrays[0];
	pArray outputArray = std::make_shared<arrayBase>(inputArray->getDesc());
	std::shared_ptr<coor> sp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	std::shared_ptr<coor> ep = std::static_pointer_cast<coor>(this->params_[2]->getParam());

	auto chunkItr = inputArray->getChunkIterator();
	while (!chunkItr->isEnd())
	{
		auto blockItr = (**chunkItr)->getBlockIterator();
		while (!blockItr->isEnd())
		{
			coor globalSp = chunkItr->getEp() + blockItr->getSp();
			coor globalEp = chunkItr->getEp() + blockItr->getEp();
			if ((*sp) < globalSp && globalEp < (*ep))
			{
				blockItr;
			} else
			{
				coorRange tmp(*sp, *ep);
				(**blockItr)->getItemRangeIterator(tmp);
			}
			++(*blockItr);
		}
		++(*chunkItr);
	}
	return inputArray;
}
}