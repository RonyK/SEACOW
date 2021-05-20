#pragma once
#ifndef _MSDB_OP_ADAPT_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_ADAPT_HUFFMAN_ENCODE_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <compression/adaptHuffmanChunk.h>

namespace msdb
{
class adapt_huffman_encode_action : public opAction
{
public:
	adapt_huffman_encode_action();
	virtual ~adapt_huffman_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	pAdaptHuffmanChunk makeOutChunk(pChunk inChunk);
};
}		// msdb
#endif	// _MSDB_OP_ADAPT_HUFFMAN_ENCODE_ACTION_H_