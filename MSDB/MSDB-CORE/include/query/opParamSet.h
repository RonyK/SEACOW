#pragma once
#ifndef _MSDB_OPPARAMSET_H_
#define _MSDB_OPPARAMSET_H_

#include <query/opParam.h>
#include <index/bitmap.h>
#include <vector>
#include <memory>

namespace msdb
{
class opParamSet;
using pParamSet = std::shared_ptr<opParamSet>;

class opParamSet : public std::enable_shared_from_this <opParamSet>
{
public:
	opParamSet();
	opParamSet(parameters& pSet);

public:
	//bool isMatch(parameters pSet);
	virtual pArrayDesc inferSchema() = 0;
	virtual pBitmap inferBottomUpBitmap() = 0;
	virtual pBitmap inferTopDownBitmap(pBitmap fromParent) = 0;
	parameters getParam();

protected:
	parameters params_;
};

class opArrayParamSet : public opParamSet
{
public:
	opArrayParamSet(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmap inferBottomUpBitmap() override;
	virtual pBitmap inferTopDownBitmap(pBitmap fromParent) override;
};
}

#endif	// _MSDB_OPPARAMSET_H_
