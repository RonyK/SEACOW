#include <stdafx.h>
#include <index/attributeIndex.h>

namespace msdb
{
attributeIndex::attributeIndex()
	: aiType_(attrIndexType::MMT)
{
}
attributeIndex::attributeIndex(attrIndexType type)
{
	this->aiType_ = type;
}
attrIndexType attributeIndex::getType()
{
	return this->aiType_;
}
void attributeIndex::setType(attrIndexType type)
{
	this->aiType_ = type;
}
}	// msdb
