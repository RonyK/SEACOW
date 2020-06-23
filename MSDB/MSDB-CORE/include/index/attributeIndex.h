#pragma once
#ifndef _MSDB_ATTRIBUTE_INDEX_H_
#define _MSDB_ATTRIBUTE_INDEX_H_

#include <memory>

namespace msdb
{
class attributeIndex;
using pAttributeIndex = std::shared_ptr<attributeIndex>;

enum class attributeIndexType
{
	MMT
};

class attributeIndex
{
public:
	attributeIndex();

public:
	attributeIndexType aiType_;
};
}

#endif		// _MSDB_ATTRIBUTE_INDEX_H_