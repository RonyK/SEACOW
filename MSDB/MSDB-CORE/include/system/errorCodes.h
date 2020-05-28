#pragma once
#ifndef _MSDB_ERRORCODES_H_
#define _MSDB_ERRORCODES_H_

namespace msdb
{
	enum
	{
#define X(_name, _code, _msg)	_name = _code,
#include "errorCategories.inc"
#include "errors.inc"
#undef X
	};
}

#endif