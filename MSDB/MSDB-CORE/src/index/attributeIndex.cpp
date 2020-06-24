#include <index/attributeIndex.h>

namespace msdb
{
attributeIndex::attributeIndex()
	: aiType_(attrIndexType::MMT)
{
}
attrIndexType attributeIndex::getType()
{
	return this->aiType_;
}
}	// msdb
