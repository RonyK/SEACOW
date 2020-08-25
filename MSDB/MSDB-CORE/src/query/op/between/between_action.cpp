#include <op/between/between_action.h>
#include <array/arrayMgr.h>
#include <array/blockChunk.h>
#include <array/memBlockArray.h>

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
	pArray inArr = inputArrays[0];
	pArray outArr = arrayMgr::instance()->makeArray<memBlockArray>(this->getArrayDesc());
	std::shared_ptr<coor> sp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	std::shared_ptr<coor> ep = std::static_pointer_cast<coor>(this->params_[2]->getParam());

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		auto chunkItr = inArr->getChunkIterator();
		while (!chunkItr->isEnd())
		{
			chunkId cId = chunkItr->seqPos();
			coor chunkSp = chunkItr->getSp();
			coor chunkEp = chunkItr->getEp();

			if ((*sp) < chunkEp && chunkSp < (*ep))
			{
				auto outChunk = outArr->makeChunk(attr->id_, cId);
				outChunk->makeAllBlocks();
				outChunk->bufferRef((**chunkItr));
				
				if (!((*sp) <= chunkSp && chunkEp <= (*ep)))
				{
					auto blockItr = outChunk->getBlockIterator();
					while (!blockItr->isEnd())
					{
						auto dSize = (**blockItr)->getDSize();
						coor blockSp = chunkSp + blockItr->getSp();
						coor blockEp = chunkSp + blockItr->getEp();
						coor newSp(dSize);
						coor newEp(dSize);

						bool outRange = false;
						for (int d = 0; d< dSize; d++)
						{				
							if(blockEp[d] <= (*sp)[d] || (*ep)[d] <= blockSp[d])
							{
								outRange = true;
								break;
							}

							if (blockSp[d] < (*sp)[d])
							{
								newSp[d] = (*sp)[d] - chunkSp[d];
							} else
							{
								newSp[d] = blockSp[d] - chunkSp[d];
							}

							if ((*ep)[d] < blockEp[d])
							{
								newEp[d] = (*ep)[d] - chunkEp[d];
							}
							else
							{
								newEp[d] = blockEp[d] - chunkEp[d];
							}
						}

						if (outRange)
						{
							(**blockItr)->setIep(blockItr->getSp()); // TODO:: no item block
						} else
						{
							(**blockItr)->setIsp(newSp);
							(**blockItr)->setIep(newEp);
						}

						++(*blockItr);
					}
				}
			}
			++(*chunkItr);
		}
	}
	
	return outArr;
}
}