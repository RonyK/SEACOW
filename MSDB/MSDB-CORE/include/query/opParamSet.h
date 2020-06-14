#pragma once
#ifndef _MSDB_OPPARAMSET_H_
#define _MSDB_OPPARAMSET_H_

#include <query/opParam.h>
#include <vector>
#include <memory>

namespace msdb
{
class opParamSet;

using paramSet = std::shared_ptr<opParamSet>;

class opParamSet
{
public:
	opParamSet();
	opParamSet(parameters pSet);

public:
	bool isMatch(parameters pSet);
	virtual void initParams() = 0;
	virtual arrayDesc inferSchema() = 0;

protected:
	parameters parameters_;
};
}

#endif	// _MSDB_OPPARAMSET_H_
