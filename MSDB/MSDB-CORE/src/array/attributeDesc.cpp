#include <array/attributeDesc.h>
#include <util/element.h>

namespace msdb
{
attributeDesc::attributeDesc(attributeId id, std::string name, eleType type)
	: id_(id), name_(name), type_(type)
{
	this->typeSize_ = getEleSize(type);
}

}
