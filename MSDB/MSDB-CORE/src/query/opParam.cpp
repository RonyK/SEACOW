#include <query/opParam.h>
#include <system/exceptions.h>

namespace msdb
{
void opParamArray::getParam(void* output)
{
	*reinterpret_cast<arrayDesc*>(output) = this->desc_;
}

opParamType opParamArray::type()
{
	return opParamType::ARRAY;
}

opParamAttr::opParamAttr(attributeDesc& desc)
{
	this->desc_ = desc;
}

void opParamAttr::getParam(void* output)
{
	*reinterpret_cast<attributeDesc*>(output) = this->desc_;
}

opParamType opParamAttr::type()
{
	return opParamType::ATTRIBUTE;
}

opParamDim::opParamDim(dimensionDesc& desc)
{
	this->desc_ = desc;
}

void opParamDim::getParam(void* output)
{
	*reinterpret_cast<dimensionDesc*>(output) = this->desc_;
}

opParamType opParamDim::type()
{
	return opParamType::DIMENSION;
}

opParamConst::opParamConst(stableElement& ele)
	: opParam(), ele_(ele)
{

}

void opParamConst::getParam(void* output)
{
	*reinterpret_cast<stableElement*>(output) = this->ele_;
}
opParamType opParamConst::type()
{
	return opParamType::CONST;
}
opParam::opParam()
{
}
void opParamPlaceholder::getParam(void* output)
{
	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_LOGIC_ERROR, MSDB_ER_DO_NOT_USE));
}
opParamType opParamConstPlaceholder::type()
{
	return opParamType::CONST;
}
opParamType opParamDimPlaceholder::type()
{
	return opParamType::DIMENSION;
}
opParamType opParamAttrPlaceholder::type()
{
	return opParamType::ATTRIBUTE;
}
opParamType opParamArrayPlaceholder::type()
{
	return opParamType::ARRAY;
}
}
