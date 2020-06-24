#include <array/arrayMgr.h>
#include <cassert>

namespace msdb
{
pArrayDesc arrayMgr::getArrayDesc(arrayId arrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	assert(this->arrDescs_.find(arrId) != this->arrDescs_.end());

	return this->arrDescs_[arrId];
}
pAttrIndex arrayMgr::getAttributeIndex(arrayId arrId, attributeId attrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	assert(this->attrIndies_.find(arrId) != this->attrIndies_.end());
	assert(this->attrIndies_[arrId].find(attrId) != this->attrIndies_[arrId].end());	// check attributeId exist

	return (this->attrIndies_[arrId])[attrId];
}
pDimensionIndex arrayMgr::getDimensionIndex(arrayId arrId, dimensionId dimId)
{
	assert(arrId != INVALID_ARRAY_ID);
	assert(this->dimIndies_.find(arrId) != this->dimIndies_.end());	// Check arrayId exist
	assert(this->dimIndies_[arrId].find(dimId) != this->dimIndies_[arrId].end());	// check dimensionId exist

	return (this->dimIndies_[arrId])[dimId];
}
void arrayMgr::setArrayDesc(arrayId id, pArrayDesc desc)
{
	assert(id != INVALID_ARRAY_ID);
	this->arrDescs_[id] = desc;
}
void arrayMgr::setAttributeIndex(arrayId id, attributeId attrId, pAttrIndex aIndex)
{
	assert(id != INVALID_ARRAY_ID);
	(this->attrIndies_[id])[attrId] = aIndex;
}
void arrayMgr::setDimensionIndex(arrayId id, dimensionId dimId, pDimensionIndex dIndex)
{
	assert(id != INVALID_ARRAY_ID);
	(this->dimIndies_[id])[dimId] = dIndex;
}
bool arrayMgr::init()
{
	return false;
}
}

