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

	class iobs_base
	{
		using size_type = size_t;

	public:
		////////////////////////////////////////
		// Set options
		void width(size_type w)
		{
			this->bitWidth = w;
		}

		void initWidth()
		{
			this->bitWidth = 0;
		}

	protected:
		_NODISCARD inline size_type getWidth(const size_type length) const
		{
			return (this->bitWidth) ? std::min(length, this->bitWidth) : length;
		}

	protected:
		static const unsigned char mask[9];

	protected:
		size_t bitWidth = 0;	// default: 0
	};

	template <class _Elem>
	class iobs : public iobs_base
	{
	public:
		using size_type = size_t;
		using pos_type = std::conditional_t < sizeof(_Elem) < 31, unsigned char, unsigned int > ;

	public:
		_NODISCARD const _Elem* c_str() const noexcept
		{
			return this->stream.c_str();
		}

	protected:
		std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem>> stream;
	};

	template <class _Elem, size_t _BlockBytes = sizeof(_Elem), size_t _BlockBits = sizeof(_Elem)* CHAR_BIT>
	class obitstream : public iobs<_Elem>
	{
		using size_type = size_t;
		using pos_type = std::conditional_t < sizeof(_Elem) < 31, unsigned char, unsigned int > ;

	public:
		// This function has endian problem (byte order) with numbers
		size_type fillArray(const unsigned char* c, const size_type length)
		{
			size_type i = 0, remain = this->getWidth(length);

			// Head Bit + Byte Encoding
			while (remain > CHAR_BIT)
			{
				remain -= this->fill((static_cast<unsigned __int16>(*(c + i)) >> ((remain - 8) % CHAR_BIT)) & 0xFF, CHAR_BIT);
				i = (length - remain) / CHAR_BIT;
			}

			// Tail Bit Encoding
			while (remain > 0)
			{
				remain -= this->fill(static_cast<unsigned char>(*(c + i)) >> (CHAR_BIT - remain), remain);
			}

			return length - remain;
		}

		size_type fillChar(const unsigned char c, const size_type length = CHAR_BIT)
		{
			size_type remain = this->getWidth(length);

			while (remain > 0)
			{
				remain -= this->fill(c & this->mask[remain], remain);
			}

			return length;
		}

		size_type fillLongLong(const unsigned long long c, const size_type length = CHAR_BIT * sizeof(long long))
		{
			size_type remain = this->getWidth(length);

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

	protected:
		/*
		* Here is an example of a bitstream with 4 bits block.
		*
		*  [pos]
		*   ▼ ->
		* ┌─┬─┬─┬─┐┌─┬─┬─┬─┐
		* │ 3│ 2│ 1│ 0││ 3│ 2│ 1│ 0│ bitset<4>
		* ├─┴─┴─┴─┤├─┴─┴─┴─┤
		* │	  [0]	  ││		[1]		│ vector<bitset<4>>
		* └───────┘└───────┘
		* ┌─┬─┬─┬─┐┌─┬─┬─┬─┐
		* │ 0│ 1│ 2│ 3││ 4│ 5│ 6│ 7│ global bit order
		* └─┴─┴─┴─┘└─┴─┴─┴─┘
		*/
		pos_type fill(const unsigned char c, pos_type length = 8)
		{
			assert(length <= 8);
			if (this->pos == 0)
			{
				this->stream.push_back(0x0);
				this->endBlock = reinterpret_cast<std::bitset<sizeof(_Elem) * CHAR_BIT>*>(&this->stream.back());
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

	private:
		// out: number of filled bits
		unsigned char fillBits(unsigned char c, char length)
		{
			pos_type i = length - 1;
			pos_type last = std::max(length - (8 - (unsigned char)(this->pos % 8)), 0);

			for (; i != (pos_type)(-1) && i >= last; i--, this->pos++)
			{
				if ((c >> i) & 0x1)
				{
					(this->endBlock)->set(_BlockBits - this->pos - 1);
				}
			}
			this->pos %= _BlockBits;

			return length - last;
		}

		// out: number of filled bits
		unsigned char fillByte(unsigned char c)
		{
			(*this->endBlock) |= (static_cast<size_type>(c) << (_BlockBits - this->pos - 8));
			this->pos = (this->pos + 8) % _BlockBits;

			return CHAR_BIT;
		}

	protected:
		pos_type pos = 0;			// current bit in a byte
		std::bitset<sizeof(_Elem) * CHAR_BIT>* endBlock = NULL;
	};

	template <class _Elem, size_t _BlockBytes = sizeof(_Elem), size_t _BlockBits = sizeof(_Elem)* CHAR_BIT>
	class ibitstream : public iobs<_Elem>
	{

	};

	template <class _Elem, size_t _BlockBytes = sizeof(_Elem), size_t _BlockBits = sizeof(_Elem) * CHAR_BIT>
	class bitstringstream : public obitstream<_Elem>, ibitstream<_Elem>
	{
		using size_type = size_t;
		using pos_type = std::conditional_t<sizeof(_Elem) < 31 , unsigned char, unsigned int>;

	public:
		using iobs<_Elem>::c_str;
		using iobs<_Elem>::width;
		using iobs<_Elem>::initWidth;
		using iobs<_Elem>::stream;


	public:
		bitstringstream() {}

		// Return total number of used bits
		_NODISCARD size_type size() const noexcept
		{
			if (obitstream<_Elem>::pos)
			{
				return (this->stream.size() - 1) * _BlockBits + obitstream<_Elem>::pos;
			}
			return this->stream.size() * _BlockBits;
		}

		// Return total number of bits capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->stream.size() * _BlockBits;
		}
	};

	// STRUCT TEMPLATE _BitSmanip
	// Copy from _Smanip in <iomanip>
	// iomanip requires that _Arg inherits 'ios_base'
	template <class _Arg>
	struct _BitSmanip { // store function pointer and argument value
		_BitSmanip(iobs_base& (__cdecl* _Left)(iobs_base&, _Arg), _Arg _Val) : _Pfun(_Left), _Manarg(_Val) {}

		iobs_base&(__cdecl* _Pfun)(iobs_base&, _Arg); // the function pointer
		_Arg _Manarg; // the argument value
	};

	template <class  _Elem, size_t _Bits>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const std::bitset<_Bits>& _Right)
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
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const long long _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const unsigned long long _val)
	{
		_Ostr.fillLongLong(_val, sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const unsigned int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const char _val)
	{
		_Ostr.fillChar(static_cast<const unsigned char>(_val), CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const unsigned char _val)
	{
		_Ostr.fillChar(_val, CHAR_BIT);
		return _Ostr;
	}

	template <class _Elem, class _Arg>
	obitstream<_Elem>& operator<<(obitstream<_Elem>& _Ostr, const _BitSmanip<_Arg>& _BitManip)
	{
		(*_BitManip._Pfun)(_Ostr, _BitManip._Manarg);
		return _Ostr;
	}

	//////////////////////////////
	template <class _Elem, class _Arg>
	bitstringstream<_Elem>& operator>>(bitstringstream<_Elem>& _is, char& _val)
	{
		_val = _is.getChar(_val, CHAR_BIT);
		return _is;
	}

	template <class _Elem, class _Arg>
	bitstringstream<_Elem>& operator>>(bitstringstream<_Elem>& _is, _BitSmanip<_Arg>& _BitManip)
	{
		(*_BitManip._Pfun)(_is, _BitManip._Manarg);
		return _is;
	}


	_MRTIMP2 _BitSmanip<std::streamsize> __cdecl setw(std::streamsize);

	using bstream = bitstringstream<char>;
	using u16bstream = bitstringstream<char16_t>;
	using u32bstream = bitstringstream<char32_t>;
}
#endif	// _caBitstringstream_