#pragma once
#ifndef _MSDB_ATTRIBUTE_ID_H_
#define _MSDB_ATTRIBUTE_ID_H_

#include <stdafx.h>

namespace msdb
{
typedef uint32_t attributeId;

const attributeId INVALID_ATTRIBUTE_ID = static_cast<attributeId>(~0);
}
#endif		// _MSDB_ATTRIBUTE_ID_H_
