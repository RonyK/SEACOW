#pragma once
#ifndef _MSDB_OP_ADAPT_HUFFMAN_ENCODE_PLAN_H_
#define _MSDB_OP_ADAPT_HUFFMAN_ENCODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class adapt_huffman_encode_plan : public opPlan
{
public:
	adapt_huffman_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class adapt_huffman_encode_array_pset : public opArrayParamSet
{
public:
	adapt_huffman_encode_array_pset(parameters& pSet);
};
}		// msdb
#endif	// _MSDB_OP_ADAPT_HUFFMAN_ENCODE_PLAN_H_