#pragma once
#ifndef _caBitstream_
#define _caBitstream_

#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>

template <size_t _BlockBytes>
class bitstream
{
	using Elem_ = std::uint64_t;
	using size_type = size_t;
	const size_t _BlockBits = _BlockBytes * 8;

public:
	bitstream()
	{

	}

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
	unsigned char fill(unsigned char c, char length = 8)
	{
		assert(length <= 8);
		if (this->pos == 0)
		{
			this->stream.push_back(0x0);
			this->lastBlock = &this->stream.back();
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

	void toChar(unsigned char* output, size_type length)
	{
		assert(length > (this->size() + 7) / 8);

		size_type oPos = 0;

		// Front encoding
		size_type fullBlocks = this->stream.size() - 1;
		if (this->pos == 0)
		{
			fullBlocks += 1;
		}

		for (size_type block = 0; block < fullBlocks; block++)
		{
			for (size_type i = 0; i < _BlockBytes; i++)
			{
				output[oPos++] = (this->stream[block] >> (_BlockBytes - i - 1) * 8).to_ulong() & 0xFF;
			}
		}
		
		// Tail encoding
		for (size_type i = 0; i < (this->pos + 7) / 8; i++)
		{
			output[oPos++] = ((*this->lastBlock) >> (_BlockBytes - i - 1) * 8).to_ulong() & 0xFF;
		}
	}

protected:
	unsigned char fillBits(unsigned char c, char length)
	{
		char i = length;
		char last = std::max(length - (8 - (unsigned char)(this->pos % 8)), 0);

		for (; i > last; this->pos++)
		{
			if ((c >> --i) & 0x1)
			{
				lastBlock->set(_BlockBits - this->pos - 1);
			}
		}
		this->pos %= this->_BlockBits;

		return length - last;
	}

	unsigned char fillByte(unsigned char c)
	{
		(*this->lastBlock) |= (static_cast<size_type>(c) << (_BlockBits - this->pos - 8));
		this->pos = (this->pos + 8) % this->_BlockBits;

		return 8;
	}

protected:
	std::vector<std::bitset<_BlockBytes * 8>> stream;
	std::bitset<_BlockBytes * 8>* lastBlock = NULL;
	size_t pos = 0;	// current bit in a byte
};

template <size_t _BlockBits, size_t _Bits>
bitstream<_BlockBits>& operator<<(bitstream<_BlockBits>& _Ostr, const std::bitset<_Bits>& _Right)
{
	int length = _Bits;

	// Head Bit + Byte Encoding
	while(length >= 8)
	{
		length -= _Ostr.fill((char)(_Right.to_ulong() >> (length - 8)) & 0xFF, 8);
	}

	// Tail Bit Encoding
	while(length > 0)
	{
		length -= _Ostr.fill((char)(_Right.to_ulong()) & 0xFF, length);
	}

	return _Ostr;
}

#endif	// _caBitstream_