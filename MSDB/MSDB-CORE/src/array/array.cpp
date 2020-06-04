#include <array/array.h>

namespace msdb
{
arrayBase::arrayBase()
{

}

arrayBase::arrayBase(arrayDesc& desc)
{
	this->_desc = desc;
}
}
