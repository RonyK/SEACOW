#pragma once
#ifndef _MSDB_OPMMT_DELTA_ENCODE_ARRAY_H_
#define _MSDB_OPMMT_DELTA_ENCODE_ARRAY_H_

#include <array/array.h>
#include <index/mmt.h>
#include <map>

namespace msdb
{
class mmt_delta_encode_array : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	mmt_delta_encode_array(pArrayDesc desc);

public:
	pMMT getMMT(attributeId attrId);
	void setMMT(attributeId attrId, pMMT myMMT);

protected:
	std::map<attributeId, pMMT> attrMMT;
};
}

#endif	// _MSDB_OPMMT_DELTA_ENCODE_ARRAY_H_