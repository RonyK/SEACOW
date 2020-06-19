#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_WAVELET_ENCODE_ACTION_H_

#include <compression/wavelet.h>
#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
class wavelet_encode_action : public opAction
{
public:
	wavelet_encode_action();
	virtual ~wavelet_encode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	std::list<pChunk> chunkEncode(pArray wArray, pChunk sourceChunk, 
								  pWavelet w, size_t maxLevel, pQuery q);
	std::list<pChunk> waveletLevelEncode(pChunk wChunk, pWavelet w, pQuery q);
	template<class Ty_>
	std::list<pChunk> waveletTransform(pChunk inChunk, pWavelet w, dimensionId basisDim, pQuery q)
	{
		// Setting chunkDesc for band chunk
		auto inChunkDesc = *inChunk->getDesc();
		auto bandDesc = std::make_shared<chunkDesc>(inChunkDesc);
		bandDesc->setDim(basisDim, intDivCeil(bandDesc->dims_[basisDim], 2));

		// Make chunk
		pChunk approximateChunk = std::make_shared<chunk>(std::make_shared<chunkDesc>(*bandDesc));
		pChunk detailChunk = std::make_shared<chunk>(std::make_shared<chunkDesc>(*bandDesc));

		approximateChunk->materialize();
		detailChunk->materialize();

		// Get Iterator for each chunk
		auto iit = inChunk->getItemIterator();
		auto ait = approximateChunk->getItemIterator();
		auto dit = detailChunk->getItemIterator();

		iit.setBasisDim(basisDim);
		ait.setBasisDim(basisDim);
		dit.setBasisDim(basisDim);

		iit.moveToStart();
		ait.moveToStart();
		dit.moveToStart();

		// Iterate data
		size_t length = inChunk->getDesc()->dims_[basisDim];
		size_t rows = intDivCeil(inChunk->numCells(), length);

		for (size_t r = 0; r < rows; r++)
		{
			for (size_t i = 0; i < length; i += 2)
			{
				Ty_ h = 0, g = 0;
				for (size_t j = 0; (j < w->c_) && (i + j < length); j++)
				{
					auto in = iit.getAt(j).get<Ty_>();
					auto ap = (*ait).get<Ty_>();
					auto de = (*dit).get<Ty_>();

					h += w->h_0[j] * iit.getAt(j).get<Ty_>();
					g += w->g_0[j] * iit.getAt(j).get<Ty_>();
				}

				(*ait).set<Ty_>(h);
				(*dit).set<Ty_>(g);

				++ait;
				++dit;
				iit += 2;
			}
		}

		return std::list<pChunk>({ approximateChunk, detailChunk });
	}

public:
	virtual const char* name() override;
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_ACTION_H_