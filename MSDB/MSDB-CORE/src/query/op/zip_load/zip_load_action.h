#pragma once
#ifndef _MSDB_OP_ZIP_ACTION_H_
#define _MSDB_OP_ZIP_ACTION_H_

#include <stdafx.h>
#include <query/opAction.h>
#include <compression/zipChunk.h>

namespace msdb
{
class zip_load_action : public opAction
{
public:
	zip_load_action();
	virtual ~zip_load_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, eleDefault numBins, pQuery qry);
	pZipChunk makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid, eleDefault numBins);
};
}		// msdb
#endif	// _MSDB_OP_ZIP_ACTION_H_