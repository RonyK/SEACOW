#pragma once
#ifndef _caBitstringstream_
#define _caBitstringstream_

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>
#include <iosfwd>
#include <type_traits>

namespace caWavelet
{
	union bitchar
	{
		std::bitset<8> b;
		char c;
	};

	class bitstream
	{
	protected:
		static const unsigned char mask[9];
	};

	template <class _Elem, size_t _BlockBytes = sizeof(_Elem), size_t _BlockBits = sizeof(_Elem) * CHAR_BIT>
	class bitstringstream : public bitstream
	{
		using size_type = size_t;
		using pos_type = std::conditional_t<sizeof(_Elem) < 31 , unsigned char, unsigned int>;

	public:
		bitstringstream() {}

		// Return total number of used bits
		_NODISCARD size_type size() const noexcept
		{
			if (this->pos)
			{
				return (this->stream.size() - 1) * _BlockBits + this->pos;
			}
			return this->stream.size() * _BlockBits;
		}

		// Return total number of bits capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->stream.size() * _BlockBits;
		}

	public:
		/*
		* Here is an example of a bitstream with 4 bits block.
		*
		*  [pos]
		*   ∪ ->
		* 忙式成式成式成式忖忙式成式成式成式忖
		* 弛 3弛 2弛 1弛 0弛弛 3弛 2弛 1弛 0弛 bitset<4>
		* 戍式扛式扛式扛式扣戍式扛式扛式扛式扣
		* 弛	  [0]	  弛弛		[1]		弛 vector<bitset<4>>
		* 戌式式式式式式式戎戌式式式式式式式戎
		* 忙式成式成式成式忖忙式成式成式成式忖
		* 弛 0弛 1弛 2弛 3弛弛 4弛 5弛 6弛 7弛 global bit order
		* 戌式扛式扛式扛式戎戌式扛式扛式扛式戎
		*/


		// This function has endian problem (byte order) with numbers
		size_type fillArray(const unsigned char* c, const size_type length)
		{
			size_type i = 0, remain = length;

			// Head Bit + Byte Encoding
			while (remain > CHAR_BIT)
			{
				remain -= this->fill((static_cast<unsigned __int16>(*(c + i)) >> ((remain - 8) % CHAR_BIT)) & 0xFF, CHAR_BIT);
				i = (length - remain) / CHAR_BIT;
			}

			// Tail Bit Encoding
			while (remain > 0)
			{
				remain -= this->fill(static_cast<unsigned char>(*(c+i)) >> (CHAR_BIT - remain), remain);
			}

			return length - remain;
		}
		
		size_type fillChar(const unsigned char c, const size_type length = CHAR_BIT)
		{
			size_type remain = length;

			while (remain > 0)
			{
				remain -= this->fill(c & this->mask[remain], remain);
			}

			return length;
		}

		size_type fillLongLong(const unsigned long long c, const size_type length)
		{
			size_type remain = length;

			// Head Bit + Byte Encoding
			while (remain >= CHAR_BIT)
			{
				remain -= this->fill(static_cast<unsigned char>((c >> (remain - CHAR_BIT)) & 0xFF), CHAR_BIT);
			}

			// Tail Bit Encoding
			while (remain > 0)
			{
				remain -= this->fill(c & this->mask[remain], remain);
			}

			return length;
		}

		_NODISCARD const _Elem* c_str() const noexcept
		{
			return this->stream.c_str();
		}

		////////////////////////////////////////
		// Set options
		void width(size_type w)
		{
			assert(w <= _BlockBits);

			this->bitWidth = w;
		}

	protected:
		pos_type fill(const unsigned char c, pos_type length = 8)
		{
			assert(length <= 8);
			if (this->pos == 0)
			{
				this->stream.push_back(0x0);
				this->lastBlock = reinterpret_cast<std::bitset<sizeof(_Elem) * CHAR_BIT>*>(&this->stream.back());
			}

			if (this->pos % 8 != 0 || length % 8 != 0)
			{
				return this->fillBits(c, length);
			}
			else
			{
				return this->fillByte(c);
			}
		}

		unsigned char fillBits(unsigned char c, char length)
		{
			pos_type i = length - 1;
			pos_type last = std::max(length - (8 - (unsigned char)(this->pos % 8)), 0);

			for (; i != (pos_type)(-1) && i >= last; i--, this->pos++)
			{
				if ((c >> i) & 0x1)
				{
					lastBlock->set(_BlockBits - this->pos - 1);
				}
			}
			this->pos %= _BlockBits;

			return length - last;
		}

		unsigned char fillByte(unsigned char c)
		{
			(*this->lastBlock) |= (static_cast<size_type>(c) << (_BlockBits - this->pos - 8));
			this->pos = (this->pos + 8) % _BlockBits;

			return CHAR_BIT;
		}

	protected:
		std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem>> stream;
		std::bitset<sizeof(_Elem) * CHAR_BIT>* lastBlock = NULL;
		size_t pos = 0;			// current bit in a byte
		size_t bitWidth = 0;	// default: 0
	};

	template <class  _Elem, size_t _Bits>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const std::bitset<_Bits>& _Right)
	{
		int length = _Bits;

		// Head Bit + Byte Encoding
		while (length > 8)
		{
			length -= _Ostr.fillChar((unsigned char)(_Right.to_ulong() >> (length - 8)) & 0xFF, 8);
		}

		// Tail Bit Encoding
		while (length > 0)
		{
			length -= _Ostr.fillChar((unsigned char)(_Right.to_ulong()) & 0xFF, length);
		}

		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const long long _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const unsigned long long _val)
	{
		_Ostr.fillLongLong(_val, sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const unsigned int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const char _val)
	{
		_Ostr.fillChar(static_cast<const unsigned char>(_val), CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	bitstringstream<_Elem>& operator<<(bitstringstream<_Elem>& _Ostr, const unsigned char _val)
	{
		_Ostr.fillChar(_val, CHAR_BIT);
		return _Ostr;
	}

	using bstream = bitstringstream<char>;
	using u16bstream = bitstringstream<char16_t>;
	using u32bstream = bitstringstream<char32_t>;
}
#endif	// _caBitstringstream_