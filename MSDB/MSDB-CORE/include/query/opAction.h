#pragma once
#ifndef _MSDB_OPACTION_H_
#define _MSDB_OPACTION_H_

#include <stdafx.h>
#include <array/array.h>
#include <array/arrayDesc.h>
#include <query/opParam.h>
#include <query/query.h>
#include <util/timer.h>

namespace msdb
{
class opAction;
using pAction = std::shared_ptr<opAction>;

class opAction : public std::enable_shared_from_this<opAction>
{
public:
	opAction();
	virtual ~opAction();

public:
	const pArrayDesc getArrayDesc();
	void setParams(const parameters& params);
	void setArrayDesc(pArrayDesc aDesc);
	void setPlanBitmap(pBitmap planBitmap);
	cpBitmap getPlanChunkBitmap() const;
	virtual cpBitmap getPlanBlockBitmap(chunkId cid) const;
	virtual const char* name() = 0;

	virtual pArray execute(std::vector<pArray>&inputArrays, pQuery q) = 0;

protected:
	pArrayDesc aDesc_;
	pBitmap planBitmap_;	// chunk bitmap or chunk-block bitmap tree
	//std::vector<action> sources_;
	parameters params_;
};
}

#endif		// _MSDB_OPACTION_H_