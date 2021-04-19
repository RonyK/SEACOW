#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ACTION_H_
#define _MSDB_OP_WAVELET_DECODE_ACTION_H_

#include <stdafx.h>
#include <array/blockChunk.h>
#include <compression/wavelet.h>
#include <query/opAction.h>
#include <util/math.h>
#include <util/threadUtil.h>

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
						 pWavelet w, size_t maxLevel, pQuery qry)
	{
		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		auto ocItr = outArr->getChunkIterator(iterateMode::ALL);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

		while (!ocItr->isEnd())
		{
			pChunk outChunk = outArr->makeChunk(attrDesc->id_, ocItr->seqPos());
			outChunk->bufferAlloc();

			io_service_->post(boost::bind(&wavelet_decode_action::parallelChunkDecode<Ty_>, this,
							  outArr, outChunk, inArr->getChunkIterator(),
							  ocItr->coor(),
							  w, maxLevel, qry, currentThreadId));

			++(*ocItr);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//
	}

	template <typename Ty_>
	void parallelChunkDecode(pArray outArr, pChunk outChunk, pChunkIterator icItr,
							 const coor outChunkCoor,
							 pWavelet w, size_t maxLevel, pQuery qry, const size_t parentThreadId)
	{
		auto chunkDims = outArr->getDesc()->getDimDescs()->getChunkDims();
		auto blockSpace = outArr->getDesc()->getDimDescs()->getBlockSpace();

		bool isOutChunkEmpty = true;
		std::vector<pChunk> chunks;
		//pBitmap blockBitmap = std::make_shared<bitmap>(blockSpace.area(), false);
		auto blockBitmap = outChunk->getBlockBitmap();
		auto blockItr = outChunk->getBlockIterator();
		//coorItr bSpaceItr(blockSpace);

		// Bring all chunks belong to an outChunk
		while (!blockItr->isEnd())
		{
			auto blockCoor = blockItr->coor();
			auto inChunkCoor = outChunkCoor * blockSpace + blockCoor;

			icItr->moveTo(inChunkCoor);
			//BOOST_LOG_TRIVIAL(debug) << inChunkCoor.toString();
			//if(icItr->isExist() && (**icItr) == nullptr)
			//{
			//	BOOST_LOG_TRIVIAL(debug) << "exist but null";
			//}

			if (icItr->isExist() && (**icItr) != nullptr)
			{
				//BOOST_LOG_TRIVIAL(debug) << "exist";
				assert((*icItr)->getDesc()->chunkCoor_ == inChunkCoor);

				outChunk->makeBlock(blockItr->seqPos());
				//blockBitmap->setExist(bSpaceItr.seqPos());
				chunks.push_back(**icItr);
				isOutChunkEmpty = false;
			} else
			{
				//BOOST_LOG_TRIVIAL(debug) << "null";
				chunks.push_back(nullptr);
			}
			//++bSpaceItr;
			++(*blockItr);
		}

		if (!isOutChunkEmpty)
		{
			BOOST_LOG_TRIVIAL(debug) << "[" << outChunk->getId() << "]: chunk exist";
			// --------------------
			// TODO::PARALLEL
			this->chunkDecode<Ty_>(chunks, outChunk, outChunkCoor, chunkDims,
								   blockSpace, blockBitmap, w, maxLevel, qry);
			 // --------------------

			 //auto cid = outArr->getChunkIdFromChunkCoor(outChunkCoor);
			 //outChunk->setId(cid);

		} else
		{
			BOOST_LOG_TRIVIAL(debug) << "[" << outChunk->getId() << "]: chunk empty";
			outArr->freeChunk(outChunk->getId());
			//outChunk = nullptr;
		}
	}

	template <class Ty_>
	void chunkDecode(std::vector<pChunk>& inChunkList, pChunk outChunk,
					 const coor& outChunkCoor, const dimension& chunkDims,
					 const dimension& blockSpace,
					 pBitmap blockBitmap,
					 pWavelet w, size_t maxLevel, pQuery q)
	{
		// Find an exist chunk and set inChunkDesc
		pChunkDesc inChunkDesc;
		for (blockId bid = 0; bid < inChunkList.size(); ++bid)
		{
			if (blockBitmap->isExist(bid))
			{
				inChunkDesc = inChunkList[bid]->getDesc();
				break;
			}
		}

		//pChunkDesc outChunkDesc = std::make_shared<chunkDesc>(*inChunkDesc);
		//size_t numBlocks = blockSpace.area();

		//outChunkDesc->dims_ = chunkDims;
		//outChunkDesc->chunkCoor_ = outChunkCoor;
		//outChunkDesc->sp_ = inChunkDesc->sp_;
		//outChunkDesc->ep_ = inChunkDesc->sp_ + chunkDims;
		//outChunkDesc->mSize_ *= numBlocks;

		//pChunk outChunk = std::make_shared<memBlockChunk>(outChunkDesc);
		//outChunk->bufferAlloc();
		//outChunk->makeAllBlocks();
		outChunk->replaceBlockBitmap(blockBitmap);
		auto obItr = outChunk->getBlockIterator();

		for (blockId bid = 0; bid < inChunkList.size(); ++bid)
		{
			if (blockBitmap->isExist(bid))
			{
				auto inChunk = inChunkList[bid];
				coor chunkCoor = inChunk->getChunkCoor();
				coor blockCoor = chunkCoor % blockSpace;
				obItr->moveTo(blockCoor);

				this->blockDecode<Ty_>((**obItr), inChunk->getBlock(0), w, maxLevel, q);
			}
		}
	}

	template <class Ty_>
	void blockDecode(pBlock outBlock, pBlock inBlock,
					 pWavelet w, size_t maxLevel, pQuery q)
	{
		// Copy data from inBlock
		outBlock->copy(inBlock);
		for (size_t level = maxLevel; level != (size_t)-1; --level)
		{
			this->levelDecode<Ty_>(outBlock, w, level, q);
			//std::cout << "Level Decode" << std::endl;
			//outBlock->print();
		}
	}

	template <class Ty_>
	void levelDecode(pBlock outBlock, pWavelet w, size_t level, pQuery q)
	{
		dimensionId dSize = outBlock->getDSize();
		for (dimensionId d = dSize - 1; d != (dimensionId)-1; --d)
		{
			coorRange arrRange = outBlock->getDesc()->dims_ / pow(2, level);
			this->dimensionDecode<Ty_>(outBlock, arrRange, d, w, q);
		}
	}

	template <class Ty_>
	void dimensionDecode(pBlock outBlock,
						 coorRange& encodeRange, dimensionId& basisDim,
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

		auto iit = outBlock->getItemRangeIterator(encodeRange);
		auto ait = outBlock->getItemRangeIterator(approximateRange);
		auto dit = outBlock->getItemRangeIterator(detailRange);

		iit->setBasisDim(basisDim);
		ait->setBasisDim(basisDim);
		dit->setBasisDim(basisDim);

		iit->moveToStart();
		ait->moveToStart();
		dit->moveToStart();

		// convert to ty
		Ty_* row = new Ty_[length];

		for (size_t r = 0; r < rows; ++r)
		{
			for (size_t i = 0; i < halfLength * 2; i += 2)
			{
				Ty_ y0 = (**ait).get<Ty_>();
				Ty_ y1 = (**dit).get<Ty_>();

				row[i] = y0 - std::floor(y1 / 2.0);
				row[i + 1] = y1 + row[i];

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