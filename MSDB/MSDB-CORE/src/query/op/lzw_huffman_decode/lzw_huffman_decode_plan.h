#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_
#define _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class lzw_huffman_decode_plan : public opPlan
{
public:
	lzw_huffman_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class lzw_huffman_decode_array_pset : public opArrayParamSet
{
public:
	lzw_huffman_decode_array_pset(parameters& pSet);
};
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_