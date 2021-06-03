#pragma once
#ifndef _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_
#define _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class se_huffman_encode_plan : public opPlan
{
public:
	se_huffman_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class se_huffman_encode_array_pset : public opArrayParamSet
{
public:
	se_huffman_encode_array_pset(parameters& pSet);
};
}		// msdb
#endif	// _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_