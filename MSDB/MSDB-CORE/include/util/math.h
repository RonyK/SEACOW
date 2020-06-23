#pragma once
#ifndef _MSDB_MATH_H_
#define _MSDB_MATH_H_

#include <type_traits>
#include <cstdint>
#include <cassert>

namespace msdb
{
#define SIGN(val) ((val >= 0) ? 1 : -1)

int64_t intDivCeil(int64_t li, int64_t ri);

using bit_cnt_type = unsigned char;		// number of bit
using sig_bit_type = char;				// significant bit

unsigned long long calcMaxLimit(bit_cnt_type bits);
unsigned long long calcMinLimit(bit_cnt_type bits);

	// Return Max Limit value where num of bits is provided.
template <typename Ty_>
Ty_ getMaxBoundary(sig_bit_type bits)
{
	if (bits == 0)
	{
		return 0;
	} else if (bits > 0)
	{
		return static_cast<Ty_>(calcMaxLimit(bits));
	} else
	{
		return static_cast<Ty_>(calcMinLimit(-bits) * -1);
	}
}

template <typename Ty_>
Ty_ getMinBoundary(sig_bit_type bits)
{
	if (bits == 0)
	{
		return 0;
	} else if (bits > 0)
	{
		return static_cast<Ty_>(calcMinLimit(bits));
	} else
	{
		return static_cast<Ty_>(calcMaxLimit(-bits) * -1);
	}
}

// Return Max Limit value where num of bits is provided.
template <typename Ty_>
Ty_ getMaxBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	assert(order > 1 && sigBitPos > 0);

	if (prevLimit == 0)
	{
		return 0;
	} else if (prevLimit > 0)
	{
		bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
		assert(prefixPos >= sigBitPos);

		return (prevLimit | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& calcMaxLimit(sigBitPos);
	} else
	{
		bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
		assert(prefixPos >= sigBitPos);

		return ((std::abs(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& ((Ty_)1 << (sigBitPos - 1))) * -1;
	}
}

template <typename Ty_>
Ty_ getMinBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	assert(order > 1 && sigBitPos > 0);

	if (prevLimit == 0)
	{
		return 0;
	} else if (prevLimit > 0)
	{
		bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
		assert(prefixPos >= sigBitPos);

		return (prevLimit | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& ((Ty_)1 << (sigBitPos - 1));
	} else
	{
		bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
		assert(prefixPos >= sigBitPos);

		return ((std::abs(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& calcMaxLimit(sigBitPos)) * -1;
	}
}

template<typename Ty_, typename size_type = std::conditional_t<sizeof(Ty_) < 32, unsigned char, unsigned int > ,
	size_t Bits_ = sizeof(Ty_)* CHAR_BIT>
	size_type msb(Ty_ value, size_type order = 1)
{
	assert(order > 0);

	if (value == 0)
		return 0;

	Ty_ mask = 1 << (Bits_ - 1);
	size_type i = Bits_ + 1;
	while (order != 0 && i-- != 0)
	{
		(mask & value) && --order;
		value <<= 1;
	}

	return i;
}
}

#endif		// _MSDB_MATH_H_