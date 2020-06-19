#include <query/opParam.h>
#include <system/exceptions.h>

namespace msdb
{
opParamArray::opParamArray(pArrayDesc desc)
	: desc_(desc)
{
}
opParam::void_pointer opParamArray::getParam()
{
	return this->desc_;
}

opParamType opParamArray::type()
{
	return opParamType::ARRAY;
}

opParamAttr::opParamAttr(pAttributeDesc desc)
	: desc_(desc)
{
}

opParam::void_pointer opParamAttr::getParam()
{
	return this->desc_;
}

opParamType opParamAttr::type()
{
	return opParamType::ATTRIBUTE;
}

opParamDim::opParamDim(pDimensionDesc desc)
	: desc_(desc)
{
}

opParam::void_pointer opParamDim::getParam()
{
	return this->desc_;
}

opParamType opParamDim::type()
{
	return opParamType::DIMENSION;
}

opParamConst::opParamConst(pStableElement ele)
	: opParam(), ele_(ele)
{
}

opParam::void_pointer opParamConst::getParam()
{
	return this->ele_;
}
opParamType opParamConst::type()
{
	return opParamType::CONST;
}
opParam::opParam()
{
}
opParam::void_pointer opParamPlaceholder::getParam()
{
	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_LOGIC_ERROR, MSDB_ER_DO_NOT_USE));
	return nullptr;
}
opParamConstPlaceholder::opParamConstPlaceholder()
	: opParamPlaceholder(), opParamConst(nullptr)
{
}
opParamType opParamConstPlaceholder::type()
{
	return opParamType::CONST;
}
opParamDimPlaceholder::opParamDimPlaceholder()
	: opParamPlaceholder(), opParamDim(nullptr)
{
}
opParamType opParamDimPlaceholder::type()
{
	return opParamType::DIMENSION;
}
opParamAttrPlaceholder::opParamAttrPlaceholder()
	: opParamPlaceholder(), opParamAttr(nullptr)
{
}
opParamType opParamAttrPlaceholder::type()
{
	return opParamType::ATTRIBUTE;
}
opParamArrayPlaceholder::opParamArrayPlaceholder()
	: opParamPlaceholder(), opParamArray(nullptr)
{
}
opParamType opParamArrayPlaceholder::type()
{
	return opParamType::ARRAY;
}
}
