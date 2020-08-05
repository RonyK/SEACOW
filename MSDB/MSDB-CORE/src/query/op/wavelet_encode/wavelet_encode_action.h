#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_WAVELET_ENCODE_ACTION_H_

#include <compression/wavelet.h>
#include <compression/wtChunk.h>
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
	/* chunkEncode() splits a chunk into multiple band chunks.
	*  Numbers in chunks are bnadId.
	*	┌───────────┐   ┌─────┬─────┐   ┌──┬──┬─────┐
	*	│           │   │     │     │   │ 0│ 1│     │
	*	│           │   │  0  │  1  │   ├──┼──┤  4  │
	*	│           │   │     │     │   │ 2│ 3│     │
	*	│     0     │ → ├─────┼─────┤ → ├──┴──┼─────┤
	*	│           │   │     │     │   │     │     │
	*	│           │   │  2  │  3  │   │  5  │  6  │
	*	│           │   │     │     │   │     │     │
	*	└───────────┘   └─────┴─────┘   └─────┴─────┘
	*      chunk 0         level 0         level 1
	*
	*	┌───────────┬───────────┐   ┌──┬──┬─────┬──┬──┬─────┐
	*	│           │           │   │ 0│ 1│     │ 7│ 8│     │
	*	│           │           │   ├──┼──┤  4  ├──┼──┤  11 │
	*	│           │           │   │ 2│ 3│     │ 9│10│     │
	*	│     0     │     1     │ → ├──┴──┼─────├──┴──┼─────┤
	*	│           │           │   │     │     │     │     │
	*	│           │           │   │  5  │  6  │  12 │  13 │
	*	│           │           │   │     │     │     │     │
	*	└───────────┴───────────┘   └─────┴─────┴─────┴─────┘
	*      chunk 0      chunk1               level 1
	*/
	std::list<pChunk> chunkEncode(pArray wArray, pChunk sourceChunk, 
								  pWavelet w, size_t maxLevel, pQuery q);
	std::list<pChunk> waveletLevelEncode(pChunk wChunk, pWavelet w, pQuery q);

	template<class Ty_>
	std::list<pChunk> waveletTransform(pChunk inChunk, pWavelet w, dimensionId basisDim, pQuery q)
	{
		// Setting chunkDesc for band chunk
		auto inChunkDesc = inChunk->getDesc();
		auto bandDesc = std::make_shared<chunkDesc>(*inChunkDesc);
		bandDesc->setDims(basisDim, intDivCeil(bandDesc->dims_[basisDim], 2));

		// Make chunk
		pWtChunk approximateChunk = std::make_shared<wtChunk>(std::make_shared<chunkDesc>(*bandDesc));
		pWtChunk detailChunk = std::make_shared<wtChunk>(std::make_shared<chunkDesc>(*bandDesc));

		approximateChunk->alloc();
		detailChunk->alloc();

		// Get Iterator for each chunk
		auto iit = inChunk->getItemIterator();
		auto ait = approximateChunk->getItemIterator();
		auto dit = detailChunk->getItemIterator();

		iit->setBasisDim(basisDim);
		ait->setBasisDim(basisDim);
		dit->setBasisDim(basisDim);

		iit->moveToStart();
		ait->moveToStart();
		dit->moveToStart();

		// Iterate data
		size_t length = inChunk->getDesc()->dims_[basisDim];
		size_t rows = intDivCeil(inChunk->numCells(), length);

		for (size_t r = 0; r < rows; r++)
		{
			for (size_t i = 0; i < length; i += 2)
			{
				// WARNING::
				// Intermediate value should be double
				//Ty_ h = 0, g = 0;
				double h = 0, g = 0;
				for (size_t j = 0; (j < w->c_) && (i + j < length); j++)
				{
					auto in = iit->getAt(j).get<Ty_>();
					char in_char = iit->getAt(j).getChar();

					h += w->h_0[j] * iit->getAt(j).get<Ty_>();
					g += w->g_0[j] * iit->getAt(j).get<Ty_>();
				}

				(**ait).set<Ty_>(h);
				(**dit).set<Ty_>(g);

				++(*ait);
				++(*dit);
				(*iit) += 2;
			}
		}

		return std::list<pChunk>({ 
			std::static_pointer_cast<chunk>(approximateChunk), 
			std::static_pointer_cast<chunk>(detailChunk) });
	}

public:
	virtual const char* name() override;
	std::string waveletName_;		// Do not delete this variable.
									// This for the testing various wavelet functions.
};
}
#endif		// _MSDB_OP_WAVELET_ENCODE_ACTION_H_