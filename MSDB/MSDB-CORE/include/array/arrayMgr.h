#pragma once
#ifndef _MSDB_ARRAYMGR_H_
#define _MSDB_ARRAYMGR_H_

#include <util/singleton.h>
#include <array/array.h>
#include <index/attributeIndex.h>
#include <index/dimensionIndex.h>
#include <map>

namespace msdb
{
class arrayMgr : public singleton<arrayMgr>
{
private:
	arrayMgr();

	~arrayMgr();

public:
	pArrayDesc getArrayDesc(arrayId arrId);
	pAttrIndex getAttributeIndex(arrayId arrId, attributeId attrId);
	pDimensionIndex getDimensionIndex(arrayId arrId, dimensionId dimId);

	void setArrayDesc(arrayId arrId, pArrayDesc desc);
	void setAttributeIndex(arrayId arrId, attributeId attrId, pAttrIndex aIndex);
	void setDimensionIndex(arrayId arrId, dimensionId dimId, pDimensionIndex dIndex);

public:
	std::map<arrayId, pArrayDesc> arrDescs_;
	std::map<arrayId, std::map<attributeId, pAttrIndex>> attrIndies_;
	std::map<arrayId, std::map<dimensionId, pDimensionIndex>> dimIndies_;

protected:
	friend singleton<arrayMgr>;
};
}

#endif		// _MSDB_ARRAYMGR_H_