#pragma once
#ifndef _MSDB_MATH_H_
#define _MSDB_MATH_H_

#include <cstdint>

namespace msdb
{
int64_t intDivCeil(int64_t li, int64_t ri);

//template<typename Ty_, typename size_type = std::conditional_t<sizeof(Ty_) < 32, unsigned char, unsigned int > ,
//	size_t Bits_ = sizeof(Ty_)* CHAR_BIT>
//	size_type msb(Ty_ value, size_type order = 1)
//{
//	assert(order > 0);
//
//	if (value == 0)
//		return 0;
//
//	Ty_ mask = 1 << (Bits_ - 1);
//	size_type i = Bits_ + 1;
//	while (order != 0 && i-- != 0)
//	{
//		(mask & value) && --order;
//		value <<= 1;
//	}
//
//	return i;
//}
}

#endif		// _MSDB_MATH_H_