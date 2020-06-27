#pragma once
#ifndef _MSDB_ATTRIBUTE_INDEX_H_
#define _MSDB_ATTRIBUTE_INDEX_H_

#include <memory>

namespace msdb
{
class attributeIndex;
using pAttrIndex = std::shared_ptr<attributeIndex>;

enum class attrIndexType
{
	MMT
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
}

#endif		// _MSDB_ATTRIBUTE_INDEX_H_