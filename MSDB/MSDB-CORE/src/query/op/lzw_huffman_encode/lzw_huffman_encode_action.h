#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <compression/lzwHuffmanChunk.h>

namespace msdb
{
class lzw_huffman_encode_action : public opAction
{
public:
	lzw_huffman_encode_action();
	virtual ~lzw_huffman_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	pLzwHuffmanChunk makeOutChunk(pChunk inChunk);
};
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_