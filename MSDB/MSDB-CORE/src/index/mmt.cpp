#include <index/mmt.h>

namespace msdb
{
MinMaxTree::MinMaxTree(eleType eType)
	: attributeIndex(attrIndexType::MMT), serializable(std::make_shared<mmtHeader>())
{
}
}	// msdb