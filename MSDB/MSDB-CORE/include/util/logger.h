#pragma once
#ifndef _MSDB_LOGGER_H_
#define _MSDB_LOGGER_H_

#include <stdafx.h>

namespace msdb
{
//////////////////////////////
// Trivial logging with filters
//////////////////////////////
//
// - trace
// - debug
// - info
// - warning
// - error
// - fatal
//

bool initBoostLogger();
}
#endif	// _MSDB_LOGGER_H_
 