#pragma once
#ifndef _MSDB_SE_HUFFMAN_DECODE_PLAN_H_
#define _MSDB_SE_HUFFMAN_DECODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class se_huffman_decode_plan : public opPlan
{
public:
	se_huffman_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class se_huffman_decode_array_pset : public opArrayParamSet
{
public:
	se_huffman_decode_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// msdb
#endif	// _MSDB_SE_HUFFMAN_DECODE_PLAN_H_