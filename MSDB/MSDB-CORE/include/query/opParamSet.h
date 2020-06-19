#pragma once
#ifndef _MSDB_OPPARAMSET_H_
#define _MSDB_OPPARAMSET_H_

#include <query/opParam.h>
#include <vector>
#include <memory>

namespace msdb
{
class opParamSet;

using pParamSet = std::shared_ptr<opParamSet>;

class opParamSet : public std::enable_shared_from_this <opParamSet>
{
public:
	opParamSet();
	opParamSet(parameters pSet);

public:
	bool isMatch(parameters pSet);
	virtual void initParams() = 0;
	virtual pArrayDesc inferSchema() = 0;

protected:
	parameters params_;
};
}

#endif	// _MSDB_OPPARAMSET_H_
