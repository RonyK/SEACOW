#pragma once
#ifndef _MSDB_OPACTION_H_
#define _MSDB_OPACTION_H_

#include <array/array.h>
#include <array/arrayDesc.h>
#include <query/opParam.h>
#include <query/query.h>
#include <memory>

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
	virtual const char* name() = 0;

	virtual pArray execute(std::vector<pArray>&inputArrays, pQuery q) = 0;

protected:
	pArrayDesc aDesc_;
	//std::vector<action> sources_;
	parameters params_;
};
}

#endif		// _MSDB_OPACTION_H_