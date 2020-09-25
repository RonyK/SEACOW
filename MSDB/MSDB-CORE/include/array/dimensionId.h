#pragma once
#ifndef _MSDB_DIMENSION_ID_H_
#define _MSDB_DIMENSION_ID_H_

#include <stdafx.h>

namespace msdb
{
typedef uint32_t dimensionId;

const dimensionId INVALID_DIMENSION_ID = static_cast<dimensionId>(~0);
}
#endif		// _MSDB_DIMENSION_ID_H_
