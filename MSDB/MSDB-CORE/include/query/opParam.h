#pragma once
#ifndef _MSDB_OPPARAM_H_
#define _MSDB_OPPARAM_H_

#include <array/arrayDesc.h>
#include <util/element.h>
#include <memory>

#define _MSDB_MAKE_PARAM(p)		std::shared_ptr<opParam>(new p())

namespace msdb
{
class opParam;

using pParameter = std::shared_ptr<opParam>;
using parameters = std::vector<pParameter>;

using paramSetId = size_t;


enum class opParamType
{
	ARRAY,
	ATTRIBUTE,
	DIMENSION,
	CONST
};

class opParam : public std::enable_shared_from_this<opParam>
{
public:
	using void_pointer = std::shared_ptr<void>;

public:
	opParam();

public:
	virtual void_pointer getParam() = 0;
	virtual opParamType type() = 0;
};

class opParamArray : public opParam
{
public:
	using paramType = arrayDesc;

public:
	opParamArray(pArrayDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();


private:
	pArrayDesc desc_;
};

class opParamAttr : public opParam
{
public:
	using paramType = attributeDesc;

public:
	opParamAttr(pAttributeDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pAttributeDesc desc_;
};

class opParamDim : public opParam
{
public:
	using paramType = dimensionDesc;

public:
	opParamDim(pDimensionDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pDimensionDesc desc_;
};

class opParamConst : public opParam
{
public:
	using paramType = stableElement;

public:
	opParamConst(pStableElement ele);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pStableElement ele_;
};

//////////////////////////////
// Placeholder classes
class opParamPlaceholder
{
public:
	opParamPlaceholder() = default;

public:
	virtual opParam::void_pointer getParam();
};

class opParamArrayPlaceholder : public opParamPlaceholder, public opParamArray
{
public:
	opParamArrayPlaceholder();

public:
	virtual opParamType type();
};

class opParamAttrPlaceholder : public opParamPlaceholder, public opParamAttr
{
public:
	opParamAttrPlaceholder();

public:
	virtual opParamType type();
};

class opParamDimPlaceholder : public opParamPlaceholder, public opParamDim
{
public:
	opParamDimPlaceholder();

public:
	virtual opParamType type();
};

class opParamConstPlaceholder : public opParamPlaceholder, public opParamConst
{
public:
	opParamConstPlaceholder();

public:
	virtual opParamType type();
};
}


#endif		// _MSDB_OPPARAM_H_