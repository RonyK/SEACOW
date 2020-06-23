#include <array/arrayMgr.h>
#include <cassert>

namespace msdb
{
pArrayDesc arrayMgr::getArrayDesc(arrayId id)
{
	assert(id != INVALID_ARRAY_ID);
	assert(this->arrayDescs_.find(id) != this->arrayDescs_.end());
	return this->arrayDescs_[id];
}
pAttributeIndex arrayMgr::getAttributeIndex(arrayId id)
{
	assert(id != INVALID_ARRAY_ID);
	assert(this->attributeIndies_.find(id) != this->attributeIndies_.end());
	return this->attributeIndies_[id];
}
pDimensionIndex arrayMgr::getDimensionIndex(arrayId id)
{
	assert(id != INVALID_ARRAY_ID);
	assert(this->dimensionIndies_.find(id) != this->dimensionIndies_.end());
	return this->dimensionIndies_[id];
}
void arrayMgr::setArrayDesc(arrayId id, pArrayDesc desc)
{
	assert(id != INVALID_ARRAY_ID);
	this->arrayDescs_[id] = desc;
}
void arrayMgr::setAttributeIndex(arrayId id, pAttributeIndex aIndex)
{
	assert(id != INVALID_ARRAY_ID);
	this->attributeIndies_[id] = aIndex;
}
void arrayMgr::setDimensionIndex(arrayId id, pDimensionIndex dIndex)
{
	assert(id != INVALID_ARRAY_ID);
	this->dimensionIndies_[id] = dIndex;
}
}

