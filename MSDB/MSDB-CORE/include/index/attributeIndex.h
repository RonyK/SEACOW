#pragma once
#ifndef _MSDB_ATTRIBUTE_INDEX_H_
#define _MSDB_ATTRIBUTE_INDEX_H_

#include <stdafx.h>

namespace msdb
{
enum class attrIndexType
{
	MMT,
	BIN
};

class attributeIndex
{
public:
	attributeIndex();
	attributeIndex(attrIndexType type);

public:
	attrIndexType getType();
	void setType(attrIndexType type);

private:
	attrIndexType aiType_;
};
using pAttrIndex = std::shared_ptr<attributeIndex>;
}			// msdb
#endif		// _MSDB_ATTRIBUTE_INDEX_H_