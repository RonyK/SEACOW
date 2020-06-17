#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_WAVELET_ENCODE_ACTION_H_

#include <compression/wavelet.h>
#include <query/opAction.h>
#include <vector>
#include <list>

namespace msdb
{
class wavelet_encode_action : public opAction
{
public:
	wavelet_encode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	std::list<pChunk> chunkEncode(pArray wArray, pChunk targetChunk,
								  pQuery q, pWavelet w, size_t maxLevel);
	std::list<pChunk> waveletLevelEncode(pChunk wChunk, pQuery q, pWavelet w);
	std::list<pChunk> waveletTransform(pChunk inChunk, pQuery q, pWavelet w);

public:
	virtual const char* name() override;
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_ACTION_H_