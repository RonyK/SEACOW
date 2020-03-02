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

	// Return total number of bits
	size_type size()
	{
		if (this->pos)
		{
			return (this->stream.size() - 1) * _BlockBits + this->pos;
		}
		return this->stream.size() * _BlockBits;
	}

public:
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

		// Front body encoding
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
			//std::bitset<8> b1 = ((*this->lastBlock) >> (_BlockBytes - i - 1) * 8);
			//char a = b1.to_ulong();
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
		(*this->lastBlock) |= (c << (_BlockBits - this->pos - 8));
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


	//int i = _Bits - 8;
	//char length = 8;
	//if (i < 0)
	//{
	//	i = 0;
	//	length = _Bits;
	//}
	//do 
	//{
	//	i -= _Ostr.fill((char)(_Right.to_ulong() >> i) & 0xFF, length);
	//	if (i - 8 < 0)
	//	{
	//		length = i;
	//	}
	//} while (i > 0);



	//// Fill head
	//int i = _Bits;

	//if (i > 8)
	//{
	//	i -= _Ostr.fillBits((char)((_Right.to_ulong() >> (i - 8)) & 0xFF), 8);
	//}

	//// Fill mid
	//for (; i - 8 >= 0; i -= 8)
	//{
	//	_Ostr.fillByte((char)(_Right.to_ulong() >> i) & 0xFF);
	//}

	//// Fill tail
	//if (i != 0)
	//{
	//	_Ostr.fillBits((char)(_Right.to_ulong()) & 0xFF, i);
	//}

	return _Ostr;
}

#endif	// _caBitstream_