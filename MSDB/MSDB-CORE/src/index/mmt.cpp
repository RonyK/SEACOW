#include <index/mmt.h>

namespace msdb
{
MinMaxTree::MinMaxTree(eleType eType, size_const maxLevel)
	: attributeIndex(attrIndexType::MMT), serializable(std::make_shared<mmtHeader>()),
	eType_(eType), maxLevel_(maxLevel)
{

}

eleType MinMaxTree::getEleType()
{
	return this->eType_;
}

MinMaxTree::size_type MinMaxTree::getMaxLevel()
{
	return this->maxLevel_;
}
}	// msdb