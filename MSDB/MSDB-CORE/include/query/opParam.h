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
	opParam();

public:
	virtual void getParam(void* output) = 0;
	virtual opParamType type() = 0;
};

class opParamArray : public opParam
{
	opParamArray(arrayDesc& desc);

public:
	virtual void getParam(void* output);
	virtual opParamType type();

private:
	arrayDesc desc_;
};

class opParamAttr : public opParam
{
public:
	opParamAttr(attributeDesc& desc);

public:
	virtual void getParam(void* output);
	virtual opParamType type();

private:
	attributeDesc desc_;
};

class opParamDim : public opParam
{
public:
	opParamDim(dimensionDesc& desc);

public:
	virtual void getParam(void* output);
	virtual opParamType type();

private:
	dimensionDesc desc_;
};

class opParamConst : public opParam
{
public:
	opParamConst(stableElement& ele);

public:
	virtual void getParam(void* output);
	virtual opParamType type();

private:
	stableElement ele_;
};

//////////////////////////////
// Placeholder classes
class opParamPlaceholder : public opParam
{
public:
	void getParam(void* output);
};

class opParamArrayPlaceholder : public opParamPlaceholder
{
public:
	virtual opParamType type();
};

class opParamAttrPlaceholder : public opParamPlaceholder
{
public:
	virtual opParamType type();
};

class opParamDimPlaceholder : public opParamPlaceholder
{
public:
	virtual opParamType type();
};

class opParamConstPlaceholder : public opParamPlaceholder
{
public:
	virtual opParamType type();
};
}


#endif		// _MSDB_OPPARAM_H_