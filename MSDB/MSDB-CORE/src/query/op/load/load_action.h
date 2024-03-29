 #pragma once
#ifndef _MSDB_OP_LOAD_ACTION_H_
#define _MSDB_OP_LOAD_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <query/opAction.h>
#include <index/mmt.h>

namespace msdb
{
namespace core
{
class load_action : public opAction
{
public:
	load_action();
	~load_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pArray outArr, pChunk outChunk, attributeId attrId, pQuery qry, const size_t parentThreadId);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LOAD_ACTION_H_