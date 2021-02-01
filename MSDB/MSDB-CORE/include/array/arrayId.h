#pragma once
#ifndef _MSDB_ARRAY_ID_H_
#define _MSDB_ARRAY_ID_H_

#include <cstdint>

namespace msdb
{
typedef uint32_t arrayId;

const arrayId INVALID_ARRAY_ID = static_cast<arrayId>(~0);
}
#endif		// _MSDB_ARRAY_ID_H_