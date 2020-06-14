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
using action = std::shared_ptr<opAction>;

class opAction : public std::enable_shared_from_this<opAction>
{
public:
	const arrayDesc& getSchema();
	void setParams(const parameters& params);
	virtual const char* name() = 0;

	virtual pArray execute(std::vector<pArray>&inputArrays, std::shared_ptr<query> q) = 0;

private:
	arrayDesc schema_;
	std::vector<action> sources_;
	parameters params_;
};
}

#endif		// _MSDB_OPACTION_H_