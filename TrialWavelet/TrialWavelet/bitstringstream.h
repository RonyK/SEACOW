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
#include "exceptions.h"

#define _TRY_CA_IO_BEGIN _TRY_BEGIN // begin try block

#define _CATCH_CA_IO_END															\
    _CATCH_ALL /* catch block */													\
		throw BitstreamIOException();												\
        /* _Myios::setstate(ios_base::badbit, true); /* set badbit and rethrow */	\
    _CATCH_END

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
		iobs_base() {}

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
			return this->bitWidth ? std::min(length, this->bitWidth) : length;
		}

	protected:
		static const unsigned char mask[9];
		static const unsigned char rmask[9];

	protected:
		size_t bitWidth = 0;	// default: 0
	};

	template <class _Block, class _Traits>
	class vector_iobs : public iobs_base
	{
	public:
		using container_type = std::vector<_Block>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;
		
		// off_type?
		// using pos_type = std::conditional_t < sizeof(_Block) < 31, unsigned char, unsigned int > ;

		vector_iobs(container_type* mem) : _container(mem), iobs_base() {};

	private:
		container_type* _container;		// memory space to store the data

	protected:
		void init(container_type* mem)
		{
			this->_container = mem;
		}

		vector_iobs() {};
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_obitstream : virtual public vector_iobs<_Block, _Traits>
	{
	public:
		using _myBase = vector_iobs<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

	public:
		vector_obitstream(container_type* mem) : _container(mem) 
		{
			_myBase::init(mem);
		}

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

			return length - remain;
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

			return length - remain;
		}

	protected:
		/*
		* Here is an example of a bitstream with 4 bits block.
		*
		*  
		* ┌───────────┐
		* │[pos]      │
		* │▼ ->		  │
		* ├──┬──┬──┬──┤┌──┬──┬──┬──┐
		* │ 3│ 2│ 1│ 0││ 3│ 2│ 1│ 0│ bitset<4>
		* ├──┴──┴──┴──┤├──┴──┴──┴──┤
		* │	[0]	      ││ [1]	   │ vector<bitset<4>>
		* └───────────┘└───────────┘
		* ┌──┬──┬──┬──┐┌──┬──┬──┬──┐
		* │ 0│ 1│ 2│ 3││ 4│ 5│ 6│ 7│ global bit order
		* └──┴──┴──┴──┘└──┴──┴──┴──┘
		*/
		pos_type fill(const unsigned char c, pos_type length = CHAR_BIT)
		{
			assert(0 < length && length <= CHAR_BIT);

			if (this->bitPos == 0)
			{
				this->addNewBlock();
			}

			if (this->bitPos % CHAR_BIT != 0 || length % CHAR_BIT != 0)
			{
				return this->fillBits(c, length);
			}
			else
			{
				return this->fillByte(c);
			}
		}

	private:
		void addNewBlock()
		{
			this->_container->push_back(0x0);
			this->endBlock = reinterpret_cast<block_bitset_type*>(&this->_container->back());
		}

		// out: number of filled bits
		unsigned char fillBits(const unsigned char c, const pos_type length)
		{
			pos_type i = length - 1;
			pos_type last = std::max((pos_type)(length - (CHAR_BIT - (char)(this->bitPos % CHAR_BIT))), (pos_type)0);

			for (; i != (pos_type)(-1) && i >= last; i--, this->bitPos++)
			{
				if ((c >> i) & 0x1)
				{
					(this->endBlock)->set(_BlockBits - this->bitPos - 1);
				}
			}
			this->bitPos %= _BlockBits;

			return length - last;
		}

		// out: number of filled bits
		unsigned char fillByte(const unsigned char c)
		{
			(*this->endBlock) |= (static_cast<size_type>(c) << (_BlockBits - this->bitPos - CHAR_BIT));
			//this->bitPos = this->bitPos + CHAR_BIT;
			this->bitPos = (this->bitPos + CHAR_BIT) % _BlockBits;

			return CHAR_BIT;
		}

	protected:
		pos_type bitPos = 0;			// current bit in a block
		block_bitset_type* endBlock = nullptr;

	private:
		container_type* _container;
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_ibitstream : virtual public vector_iobs<_Block, _Traits>
	{
	public:
		using _myBase = vector_iobs<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

		vector_ibitstream(container_type* mem) : _container(mem) 
		{
			_myBase::init(mem);
		};

	public:
		unsigned char getChar(size_type length = CHAR_BIT)
		{
			unsigned char out = 0x00;
			size_type remain = this->getWidth(length);

			while (remain > 0)
			{
				unsigned char temp = 0x00;
				remain -= this->get(temp, remain);
				out |= (temp << remain);
			}

			return out;
		}

		unsigned long long getLongLong(size_type length = CHAR_BIT * sizeof(long long))
		{
			unsigned long long out = 0x00;
			size_type remain = this->getWidth(length);

			// Head bits -> get by byte
			while (remain >= CHAR_BIT)
			{
				unsigned char temp = 0x00;
				remain -= this->get(temp, CHAR_BIT);
				out |= (temp << remain);
			}

			// Tail bits -> get by bit length
			while (remain > 0)
			{
				unsigned char temp = 0x00;
				remain -= this->get(temp, remain);
				out |= (temp << remain);
			}

			return length - remain;
		}

		bool eof()
		{
			// this->bitPos == _BlockBytes ?
			if (this->_container->size() <= this->blockPos)
			{
				return true;
			}

			return false;
		}

	protected:
		pos_type get(unsigned char& out, const pos_type length = CHAR_BIT)
		{
			assert(length <= CHAR_BIT);
			if (this->eof())
			{
				return 0;
			}

			if (this->frontBlock == nullptr)
			{
				this->frontBlock = reinterpret_cast<block_bitset_type*>(&this->_container->front());
			}

			pos_type move = 0;
			if (this->bitPos % CHAR_BIT != 0 || length % CHAR_BIT != 0)
			{
				move = this->getBits(out, length);
			}
			else
			{
				move = this->getByte(out);
			}

			if (move && this->bitPos == 0)
			{
				this->moveToNextInputBlock();
			}

			return move;
		}

	private:
		unsigned char getBits(unsigned char& out, const pos_type length)
		{
			pos_type possible = std::min(static_cast<pos_type>(_BlockBits - this->bitPos), static_cast<pos_type>(length));

			out |= (((*this->frontBlock << this->bitPos).to_ulong() & this->rmask[possible]) >> (_BlockBits - possible)) << (length - possible);
			auto blockBits = (*this->frontBlock << this->bitPos).to_ulong();

			this->bitPos = (this->bitPos + possible) % _BlockBits;

			return possible;
		}

		unsigned char getByte(unsigned char& out)
		{
			out |= ((*this->frontBlock << (_BlockBits - this->bitPos - CHAR_BIT)).to_ulong() & 0xFF);
			this->bitPos = (this->bitPos + CHAR_BIT) % _BlockBits;

			return CHAR_BIT;
		}

		void moveToNextInputBlock()
		{
			this->blockPos++;

			if (this->eof())
			{
				this->blockPos = this->_container->size();
				return;
			}
			
			this->frontBlock = reinterpret_cast<block_bitset_type*>(&this->_container->at(this->blockPos));
		}

	protected:	
		pos_type blockPos = 0;		// current byte in a stream
		pos_type bitPos = 0;		// current bit in a byte
		block_bitset_type* frontBlock = nullptr;

	private:
		container_type* _container;
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_iobitstream : public vector_ibitstream<_Block, _Traits>,
							   public vector_obitstream<_Block, _Traits>
	{
	public:
		using _myIs = vector_ibitstream<_Block, _Traits>;
		using _myOs = vector_obitstream<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

		vector_iobitstream(container_type* myContainer)
			: _myIs(myContainer), _myOs(myContainer)
		{ 
		}
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block) * CHAR_BIT>
	class vector_bitstringstream : public vector_iobitstream<_Block, _Traits>
	{
	public:
		using _myBase = vector_iobitstream<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

	public:
		vector_bitstringstream()
			: _myBase(_STD addressof(this->_container)) 
		{
		}

		//: _myBase(_STD addressof(_container)) {}

		// Return total number of used bits
		_NODISCARD size_type size() const noexcept
		{
			if (vector_obitstream<_Block, _Traits>::bitPos)
			{
				return (this->_container.size() - 1) * _BlockBits + vector_obitstream<_Block, _Traits>::bitPos;
			}
			return this->_container.size() * _BlockBits;
		}

		// Return total number of bits capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->_container.size() * _BlockBits;
		}

		_NODISCARD const _Block* data() const noexcept
		{
			return this->_container.data();
		}

	protected:
		container_type _container;
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

	template <class  _Block, class _Traits, size_t _Bits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const std::bitset<_Bits>& _Right)
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

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const long long _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const unsigned long long _val)
	{
		_Ostr.fillLongLong(_val, sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const unsigned int _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const char _val)
	{
		_Ostr.fillChar(static_cast<const unsigned char>(_val), CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const unsigned char _val)
	{
		_Ostr.fillChar(_val, CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits, class _Arg>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const _BitSmanip<_Arg>& _BitManip)
	{
		(*_BitManip._Pfun)(_Ostr, _BitManip._Manarg);
		return _Ostr;
	}

	//////////////////////////////
	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, char& _val)
	{
		_val = (char)_is.getChar();
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, unsigned char& _val)
	{
		_val = (char)_is.getChar();
		return _is;
	}

	template <class _Block, class _Traits, class _Arg>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, _BitSmanip<_Arg>& _BitManip)
	{
		(*_BitManip._Pfun)(_is, _BitManip._Manarg);
		return _is;
	}

	template <class _Elem, class _IntType>
	struct _BlockTraits
	{
		using char_type = _Elem;
		using int_type = _IntType;
		using pos_type = long long;

		static _CONSTEXPR17 void assign(_Elem& _Left, const _Elem& _Right) noexcept
		{
			_Left = _Right;
		}

		_NODISCARD static constexpr bool eq(const _Elem& _Left, const _Elem& _Right) noexcept
		{
			return _Left == _Right;
		}

		_NODISCARD static constexpr bool lt(const _Elem& _Left, const _Elem& _Right) noexcept
		{
			return _Left < _Right;
		}

		_NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept
		{
			return static_cast<_Elem>(_Meta);
		}

		_NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept
		{
			return static_cast<int_type>(_Ch);
		}

		_NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept
		{
			return _Left == _Right;
		}
	};

	template <class _Elem>
	struct _charBlockTraits : _BlockTraits<char, unsigned long>
	{

	};

	template <>
	struct _charBlockTraits<unsigned char> : _BlockTraits<unsigned char, unsigned char> {};

	template <>
	struct _charBlockTraits<char16_t> : _BlockTraits<char, unsigned long> {};
	
	template <>
	struct _charBlockTraits<char32_t> : _BlockTraits<char, unsigned int> {};

	_MRTIMP2 _BitSmanip<std::streamsize> __cdecl setw(std::streamsize);

	using bstream = vector_bitstringstream<unsigned char, _charBlockTraits<unsigned char>>;
	using u16bstream = vector_bitstringstream<char16_t, _charBlockTraits<char16_t>>;
	using u32bstream = vector_bitstringstream<char32_t, _charBlockTraits<char32_t>>;
}
#endif	// _caBitstringstream_w