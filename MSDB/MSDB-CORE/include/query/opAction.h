#pragma once
#ifndef _MSDB_OPACTION_H_
#define _MSDB_OPACTION_H_

#include <array/arrayDesc.h>

namespace msdb
{
class opAction
{
public:
	const arrayDesc& getSchema();
	virtual const char* name() = 0;

protected:
	virtual void initSchema() = 0;		// build my plane schema

private:
	arrayDesc schema_;
	std::vector<opAction*> sources_;
};
}

#endif		// _MSDB_OPACTION_H_