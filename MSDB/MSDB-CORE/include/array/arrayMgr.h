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
public:
	pArrayDesc getArrayDesc(arrayId id);
	pAttributeIndex getAttributeIndex(arrayId id);
	pDimensionIndex getDimensionIndex(arrayId id);

	void setArrayDesc(arrayId id, pArrayDesc desc);
	void setAttributeIndex(arrayId id, pAttributeIndex aIndex);
	void setDimensionIndex(arrayId id, pDimensionIndex dIndex);

public:
	std::map<arrayId, pArrayDesc> arrayDescs_;
	std::map<arrayId, pAttributeIndex> attributeIndies_;
	std::map<arrayId, pDimensionIndex> dimensionIndies_;
};
}

#endif		// _MSDB_ARRAYMGR_H_