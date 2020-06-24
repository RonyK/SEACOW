#pragma once
#ifndef _MSDB_MATH_H_
#define _MSDB_MATH_H_

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <cstdlib>

namespace msdb
{
#define SIGN(val) ((val >= 0) ? 1 : -1)

// Use abs_ instead of std::abs
// std::abs make compile error
//  - C2668: ambigous call to overloeaded function.
// As std::abs has no function for unsigned integer types.
// When we use template, we don't know what 'Ty_' type is.
// Just try to make an absolute value for all 'Ty_' types.

_NODISCARD int abs_(int num) noexcept;
_NODISCARD long int abs_(long int num) noexcept;
_NODISCARD long long int abs_(long long int num) noexcept;
_NODISCARD float abs_(float num) noexcept;
_NODISCARD double abs_(double num) noexcept;
_NODISCARD unsigned int abs_(unsigned int num) noexcept;
_NODISCARD unsigned long int abs_(unsigned long int num) noexcept;
_NODISCARD unsigned long long int abs_(unsigned long long int num) noexcept;

int64_t intDivCeil(int64_t li, int64_t ri);

using bit_cnt_type = unsigned char;		// number of bit
using sig_bit_type = char;				// significant bit

unsigned long long calcMaxLimit(bit_cnt_type bits);
unsigned long long calcMinLimit(bit_cnt_type bits);

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

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_ prevLimit, bit_cnt_type order)
{
	/*
	*  Here is an example:
	*
	*  Suppose prevLimit (1001 1111), order (3), significantBit (3)
	*  ┌──┬──┬──┬──┬──┬──┬──┬──┐
	*  │ 1│ 0│ 0│ 1│ 1│ 1│ 1│ 1│
	*  └──┴──┴──┴──┴──┴──┴──┴──┘
	*               ▲      ▲
	*               pL     sB
	*  ==================================
	*  Result:
	*  ┌──┬──┬──┬──┬──┬──┬──┬──┐
	*  │ 1│ 0│ 0│ 1│ 0│ 0│ 1│ 1│
	*  ├──┴──┴──┴──┼──┴──┼──┴──┤
	*  │prefix_copy│ 0's │ 1's │
	*  └───────────┴─────┴─────┘
	*               ▲      ▲
	*               pL     sB
	*/
	assert(order > 0);

	Ty_ absPrevLimit = abs_(prevLimit);
	sig_bit_type bits = msb<Ty_>(absPrevLimit);
	Ty_ mask = (unsigned long long)1 << (bits - 1);
	size_t prefixPos = bits;

	while (prefixPos > 0 && order > 1)
	{
		(absPrevLimit & mask) && --order;
		--prefixPos;
		mask >>= 1;
	}
	return prefixPos;
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

		return ((abs_(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& ((Ty_)1 << (sigBitPos - 1))) * -1;
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

		return ((abs_(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& calcMaxLimit(sigBitPos)) * -1;
	}
}
}	// msdb

#endif		// _MSDB_MATH_H_