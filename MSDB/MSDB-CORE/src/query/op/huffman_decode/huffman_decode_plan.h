#pragma once
#ifndef _MSDB_HUFFMAN_DECODE_PLAN_H_
#define _MSDB_HUFFMAN_DECODE_PLAN_H_

#include <stdafx.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
class huffman_decode_plan : public opPlan
{
public:
	huffman_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class huffman_decode_array_pset : public opArrayParamSet
{
public:
	huffman_decode_array_pset(parameters& pSet);
};
}		// msdb
#endif	// _MSDB_HUFFMAN_DECODE_PLAN_H_