#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ACTION_H_
#define _MSDB_OP_WAVELET_DECODE_ACTION_H_

#include <array/memChunk.h>
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
	pChunk chunkDecode(pArray wArray, std::list<pChunk>& sourceChunks,
								  pWavelet w, size_t maxLevel, pQuery q)
	{
		std::list<pChunk> levelChunks;
		auto sit = sourceChunks.begin();
	
		// Insert an approximate chunk
		levelChunks.push_back(*sit);
		++sit;

		size_t numLevelChunks = pow(2, wArray->getDesc()->dimDescs_.size()) - 1;

		for (size_t level = maxLevel; level != (size_t)-1; --level)
		{
			// Insert detail chunks
			for (size_t i = 0; i < numLevelChunks; i++)
			{
				levelChunks.push_back(*sit);
				++sit;
			}
			levelChunks.push_front(this->waveletLevelDecode<Ty_>(levelChunks, w, q));
		}

		// Assign new chunk Id through its item start point coordinate
		levelChunks.front()->setId(wArray->getChunkIdFromItemCoor(levelChunks.front()->getDesc()->sp_));
		return levelChunks.front();
	}
	template <class Ty_>
	pChunk waveletLevelDecode(std::list<pChunk>& levelChunks, pWavelet w, pQuery q)
	{
		for (dimensionId d = this->aDesc_->dimDescs_.size() - 1; d != (dimensionId)-1 ; --d)
		{
			size_t numLevelChunks = levelChunks.size();
			assert(numLevelChunks % 2 == 0);		// number of level chunks should be odd.

			try
			{
				for (size_t i = 0; i < numLevelChunks / 2; ++i)
				{
					pChunk appChunk = levelChunks.front();
					levelChunks.pop_front();
					pChunk detailChunk = levelChunks.front();
					levelChunks.pop_front();
					pChunk convChunk = this->waveletDeTransform<Ty_>(appChunk, detailChunk, w, d, q);
					levelChunks.push_back(convChunk);
				}
			}catch(...)
			{
				std::cout << "exception" << std::endl;
			}
		}

		return levelChunks.front();
	}

	template<class Ty_>
	pChunk waveletDeTransform(pChunk approximateChunk, pChunk detailChunk, pWavelet w, dimensionId basisDim, pQuery q)
	{
		// Setting outChunkDesc through bandDesc
		auto bandDesc = *(approximateChunk->getDesc());
		auto outChunkDesc = std::make_shared<chunkDesc>(bandDesc);
		outChunkDesc->setDim(basisDim, outChunkDesc->dims_[basisDim] * 2);

		// Make chunk
		pChunk outChunk = std::make_shared<memChunk>(outChunkDesc);
		outChunk->alloc();

		auto ait = approximateChunk->getItemIterator();
		auto dit = detailChunk->getItemIterator();
		auto oit = outChunk->getItemIterator();

		ait.setBasisDim(basisDim);
		dit.setBasisDim(basisDim);
		oit.setBasisDim(basisDim);

		ait.moveToStart();
		dit.moveToStart();
		oit.moveToStart();

		// Iterate data
		size_t bandBasisDimLength = approximateChunk->getDesc()->dims_[basisDim];
		size_t decodedBasisDimLength = bandBasisDimLength << 1;
		size_t filterSize = w->c_;
		size_t rows = intDivCeil(approximateChunk->numCells(), bandBasisDimLength);

		for(size_t r = 0; r < rows; r++)
		{
			std::vector<double> intermediate(filterSize, 0);
			for(size_t i = 0; i < bandBasisDimLength; i++)
			{
				Ty_ vApproximate, vDetail;
				(*ait).getData(&vApproximate);
				(*dit).getData(&vDetail);

				for(size_t j = 0; (j < filterSize) && (i + j < decodedBasisDimLength); ++j)
				{
					intermediate[(i + j) % filterSize] += w->h_1[j] * vApproximate + w->g_1[j] * vDetail;
					std::cout << intermediate[(i + j) % filterSize] << std::endl;
				}

				(*oit).set<Ty_>(intermediate[i]);
				++oit;
				(*oit).set<Ty_>(intermediate[(i + 1) % filterSize]);
				++oit;

				//std::cout << "------" << std::endl;
				//std::cout << "Intermediate" << std::endl;
				//for(int im = 0; im < filterSize; im++)
				//{
				//	std::cout << intermediate[im] << ", ";
				//}
				//std::cout << std::endl << "------" << std::endl;

				intermediate[i] = intermediate[(i + 1) % filterSize] = 0;
				++ait;
				++dit;
			}
		}

		std::cout << "approximate chunk:" << std::endl;
		approximateChunk->print();

		std::cout << "detail chunk:" << std::endl;
		detailChunk->print();

		std::cout << "outChunk:" << std::endl;
		outChunk->print();
		return outChunk;
	}

public:
	std::string waveletName_;	// This for the testing various wavelet functions.
};

}

#endif		// _MSDB_OP_WAVELET_DECODE_ACTION_H_