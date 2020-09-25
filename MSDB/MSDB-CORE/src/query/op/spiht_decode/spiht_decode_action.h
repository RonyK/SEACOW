#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_ACTION_H
#define _MSDB_OP_SPIHT_DECODE_ACTION_H

#include <stdafx.h>
#include <query/opAction.h>
#include <util/math.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/spihtChunk.h>

namespace msdb
{
class spiht_decode_action : public opAction
{
public:
	spiht_decode_action();
	virtual ~spiht_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& outputArrays, pQuery qry);
	void decodeAttribute(std::shared_ptr<wavelet_encode_array>outArr, pAttributeDesc attrDesc, pQuery qry);

	pSpihtChunk makeInChunk(std::shared_ptr<wavelet_encode_array>arr, pAttributeDesc attrDesc, 
							chunkId cid);
};
}


#endif // _MSDB_OP_SPIHT_DECODE_ACTION_H
