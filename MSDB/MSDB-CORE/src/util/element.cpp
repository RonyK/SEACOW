#include <util/element.h>
#include <system/exceptions.h>

namespace msdb
{
eleSize getEleSize(eleType type)
{
	switch(type)
	{
	case eleType::EMPTY:
		return 0;
	case eleType::CHAR:
		return sizeof(char);
	case eleType::INT8:
		return sizeof(int8_t);
	case eleType::INT16:
		return sizeof(int16_t);
	case eleType::INT32:
		return sizeof(int32_t);
	case eleType::INT64:
		return sizeof(int64_t);
	case eleType::UINT8:
		return sizeof(uint8_t);
	case eleType::UINT16:
		return sizeof(uint16_t);
	case eleType::UINT32:
		return sizeof(uint32_t);
	case eleType::UINT64:
		return sizeof(uint64_t);
	case eleType::FLOAT:
		return sizeof(float);
	case eleType::DOUBLE:
		return sizeof(double);
	}
	
	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_UNKNOWN_ELEMENT_TYPE));
}
element::element(void* ptr, eleType type)
	: ptr_(ptr)
{
	this->getFunc = this->findGetFunc(type);
}

element::gFunc element::findGetFunc(eleType type)
{
	static void(element:: * eleGetDataFPointer[13])(void*) = {
		nullptr,		// empty
		&element::getData_Bool,	// bool
		&element::getData_1Byte,	// char
		&element::getData_1Byte,	// int8
		&element::getData_2Byte,	// int16
		&element::getData_4Byte,	// int32
		&element::getData_8Byte,	// int64
		&element::getData_1Byte,	// uint8
		&element::getData_2Byte,	// uint16
		&element::getData_4Byte,	// uint32
		&element::getData_8Byte,	// uint64
		&element::getData_4Byte,	// float		WARNING
		&element::getData_8Byte,	// double		WARNING
	};

	return eleGetDataFPointer[static_cast<int>(type)];
}
}
