#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ACTION_H_
#define _MSDB_OP_WAVELET_DECODE_ACTION_H_

#include <array/blockChunk.h>
#include <compression/wavelet.h>
#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
class wavelet_decode_action : public opAction
{
public:
	wavelet_decode_action();
	virtual ~wavelet_decode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);
	virtual const char* name() override;

private:
	template <class Ty_>
	void attributeDecode(pArray outArr, pArray inArr, pAttributeDesc attrDesc,
						 pWavelet w, size_t maxLevel, pQuery q)
	{
		auto cItr = inArr->getChunkIterator();
		auto chunkDims = outArr->getDesc()->getDimDescs().getChunkDims();
		auto blockSpace = outArr->getDesc()->getDimDescs().getBlockSpace();
		size_t numBlocks = blockSpace.area();

		while (!cItr->isEnd())
		{
			std::vector<pChunk> chunks;
			for (size_t i = 0; i < numBlocks; ++i)
			{
				chunks.push_back((**cItr));
				++(*cItr);
			}
			// --------------------
			// TODO::PARALLEL
			auto outChunk = this->chunkDecode<Ty_>(chunks, chunkDims, w, maxLevel, q);
			// --------------------
			outArr->insertChunk(outChunk);
		}
	}

	template <class Ty_>
	pChunk chunkDecode(std::vector<pChunk>& inChunkList, dimension chunkDims,
					   pWavelet w, size_t maxLevel, pQuery q)
	{
		auto inChunkDesc = inChunkList[0]->getDesc();
		pChunkDesc outChunkDesc = std::make_shared<chunkDesc>(*inChunkDesc);
		dimension blockSpace = chunkDims / inChunkDesc->dims_;	// chunkDims == blockDims in outBlock
		size_t numBlocks = blockSpace.area();

		outChunkDesc->dims_ = chunkDims;
		outChunkDesc->chunkCoor_ = inChunkDesc->sp_ / chunkDims;
		outChunkDesc->sp_ = inChunkDesc->sp_;
		outChunkDesc->ep_ = inChunkDesc->sp_ + chunkDims;
		outChunkDesc->mSize_ *= numBlocks;

		pChunk outChunk = std::make_shared<memBlockChunk>(outChunkDesc);
		outChunk->bufferAlloc();
		outChunk->makeAllBlocks();
		auto obItr = outChunk->getBlockIterator();

		for (auto inChunk : inChunkList)
		{
			coor chunkCoor = inChunk->getChunkCoor();
			coor blockCoor = chunkCoor / blockSpace;
			obItr->moveTo(blockCoor);

			//coordinate outBlock->;
			this->blockDecode<Ty_>((**obItr), inChunk->getBlock(0), w, maxLevel, q);
		}

		return outChunk;
	}

	template <class Ty_>
	void blockDecode(pBlock outBlock, pBlock inBlock,
					 pWavelet w, size_t maxLevel, pQuery q)
	{
		// Copy data from inBlock
		outBlock->copy(inBlock);
		for (size_t level = 0; level <= maxLevel; ++level)
		{
			this->levelDecode<Ty_>(outBlock, w, level, q);
		}
	}

	template <class Ty_>
	void levelDecode(pBlock outBlock, pWavelet w, size_t level, pQuery q)
	{
		dimensionId dSize = outBlock->getDSize();
		for (dimensionId d = 0; d < dSize; ++d)
		{
			coorRange arrRange = outBlock->getDesc()->dims_ / pow(2, level);
			this->dimensionDecode<Ty_>(outBlock, arrRange, d, w, q);
		}
	}

	template <class Ty_>
	void dimensionDecode(pBlock outBlock,
						 coorRange encodeRange, dimensionId basisDim,
						 pWavelet w, pQuery q)
	{
		size_t length = encodeRange.getEp()[basisDim];
		size_t halfLength = length / 2;
		bool oddLength = (length % 2) != 0;
		bool singleConvFlag = (length % 4) >= 2;
		size_t rows = encodeRange.getEp().area() / length;

		auto approximateRange = encodeRange;
		auto detailRange = encodeRange;
		approximateRange.getEp()[basisDim] /= 2;
		detailRange.getSp()[basisDim] = approximateRange.getEp()[basisDim];

		assert(approximateRange.getEp()[basisDim] == length / 2);
		assert(detailRange.getSp()[basisDim] == approximateRange.getEp()[basisDim]);

		auto iit = outBlock->getItemIterator();
		auto ait = outBlock->getItemRangeIterator(approximateRange);
		auto dit = outBlock->getItemRangeIterator(detailRange);

		iit->setBasisDim(basisDim);
		ait->setBasisDim(basisDim);
		dit->setBasisDim(basisDim);

		iit->moveToStart();
		ait->moveToStart();
		dit->moveToStart();

		// convert to ty
		double* row = new double[length];

		for (size_t r = 0; r < rows; ++r)
		{
			for (size_t i = 0; i < halfLength * 2; i += 2)
			{
				row[i] = (**ait).get<Ty_>() + (**dit).get<Ty_>();
				row[i + 1] = (**ait).get<Ty_>() - (**dit).get<Ty_>();
				++(*ait);
				++(*dit);
			}

			// halfLength * 2 -> Even number
			for (size_t i = 0; i < halfLength * 2; ++i)
			{
				(**iit).set<Ty_>(static_cast<Ty_>(row[i]));
				++(*iit);
			}

			if (oddLength)
			{
				++(*iit);
				++(*ait);
				++(*dit);
			}
		}

		delete[] row;
	}

public:
	std::string waveletName_;	// This for the testing various wavelet functions.
};
}

#endif		// _MSDB_OP_WAVELET_DECODE_ACTION_H_