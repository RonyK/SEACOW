#pragma once
#ifndef _MSDB_FIXED_HUFFMAN_CODER_H_
#define _MSDB_FIXED_HUFFMAN_CODER_H_

#include <stdafx.h>
#include <boost/math/distributions/normal.hpp>
#include <compression/huffmanCode.h>
#include <util/singleton.h>

namespace msdb
{
class iFixedHuffmanCoder
{
public:
	virtual void encode(bstream& out, const void* in, size_t len) = 0;
	virtual size_t decode(void* outData, size_t lenOut, bstream& in) = 0;
	virtual size_t getCodeBitWidth() const = 0;
};

template <typename codeType, typename symbolType>
class fixedHuffmanCoderBase : public huffmanCoder<codeType, symbolType>, public iFixedHuffmanCoder
{
public:
	using parentType = huffmanCoder<codeType, symbolType>;
	using nodeType = huffmanCoder<codeType, symbolType>::huffmanNode;

public:
	fixedHuffmanCoderBase()
		: huffmanCoder<codeType, symbolType>()
	{
		this->initHuffmanCode();
	}

	fixedHuffmanCoderBase(size_t bits)
		: huffmanCoder<codeType, symbolType>(bits)
	{
		this->initHuffmanCode();
	}

	~fixedHuffmanCoderBase()
	{

	}

protected:
	void initHuffmanCode()
	{
		std::vector<size_t> freq(pow(2, this->bits_), 0);
		this->initFrequencies(freq);
		this->buildTree(freq);
		buildEncodeDecodeTable();

		BOOST_LOG_TRIVIAL(debug) << "Fixed Huffman Coder<" << this->bits_ << "> is initialized.";
	}

private:
	void initFrequencies(std::vector<size_t>& freq)
	{
		size_t range = pow(2, this->bits_ - 1);
		//size_t half = ceil(range / 2.0);
		size_t precision = 100000;
		double mean = 0;
		//double sdNarrow = std::max({ (size_t)this->bits_ - 1, (size_t)1 });
		double sdNarrow = (double)this->bits_ / 2.0;
		double sdWide = sdNarrow * 10;

		boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdNarrow);
		boost::math::normal_distribution<double, boost::math::policies::policy<>> ndWide(mean, sdWide);

		freq[0] = (boost::math::pdf(ndNarrow, 0.0) + boost::math::pdf(ndWide, 0.0)) * precision;
		freq[range] = 1;
		BOOST_LOG_TRIVIAL(debug) << "Freq==>narrow: " << sdNarrow << ", wide: " << sdWide << ", range: " << range;;
		for (size_t x = 1; x < range; ++x)
		{
			freq[x] = (boost::math::pdf(ndNarrow, (double)x) + boost::math::pdf(ndWide, (double)x)) * precision;
			if (freq[x] == 0)
			{
				freq[x] = 1;
			}
			freq[x + range] = freq[x];
			//BOOST_LOG_TRIVIAL(debug) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "+" << boost::math::pdf(ndWide, (double)x) << "=" << freq[x];
		}

		this->printFreq(freq);
	}

	void printFreq(std::vector<size_t>& freq)
	{
		BOOST_LOG_TRIVIAL(debug) << "Freq=====";
		for (size_t i = 0; i < freq.size(); ++i)
		{
			BOOST_LOG_TRIVIAL(debug) << "[" << i << "]: " << freq[i];
		}
	}

	void buildEncodeDecodeTable()
	{
		if (this->root_->isLeaf())
		{
			this->root_->codeLen_ = 1;
			this->encodeLookupTable_[this->root_->symbol_] = this->root_;
			this->insertSymbolInDecodeLookupTable(this->root_);
		}
		else
		{
			std::stack<nodeType*> dfsStack;
			dfsStack.push(this->root_);
			this->root_->code_ = 0;
			this->root_->codeLen_ = 0;

			while (!dfsStack.empty())
			{
				nodeType* node = dfsStack.top();
				dfsStack.pop();

				assert(node != nullptr);

				if (node->isLeaf())
				{
					this->encodeLookupTable_[node->symbol_] = node;
					this->insertSymbolInDecodeLookupTable(node);
				}
				else
				{
					node->left_->code_ = node->code_ << 1;
					node->right_->code_ = node->code_ << 1 | (codeType)0x1;
					node->left_->codeLen_ = node->codeLen_ + (codeType)1;
					node->right_->codeLen_ = node->codeLen_ + (codeType)1;

					dfsStack.push(node->left_);
					dfsStack.push(node->right_);
				}
			}
		}
	}

public:
	void encode(bstream& out, const symbolType* in, size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			this->encodeSymbol(out, in[i]);
		}
	}

	void decode(symbolType* outData, size_t lenOut, bstream& in)
	{
		codeType code = 0x0;
		in >> setw(parentType::bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = (code << result.second) | nextCode;
		}
	}

	virtual void encode(bstream& out, const void* in, size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			this->encodeSymbol(out, static_cast<const symbolType*>(in)[i]);
		}
	}

	virtual size_t decode(void* outData, size_t lenOut, bstream& in)
	{
		size_t readBits = 0;
		codeType code = 0x0;
		in >> setw(parentType::bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			static_cast<symbolType*>(outData)[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = (code << result.second) | nextCode;

			readBits += result.second;
			//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(result.first) << "/ L: " << static_cast<uint64_t>(result.second);
		}

		return readBits;
	}

	virtual size_t getCodeBitWidth() const
	{
		return parentType::bitCode;
	}
};

template <size_t _BITS>
class fixedHuffmanCoder : public fixedHuffmanCoderBase<uint16_t, uint8_t>, public singleton<fixedHuffmanCoder<_BITS>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint16_t, uint8_t>(_BITS), singleton<fixedHuffmanCoder<_BITS>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<1> : public fixedHuffmanCoderBase<uint8_t, uint8_t>, public singleton<fixedHuffmanCoder<1>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint8_t, uint8_t>(1), singleton<fixedHuffmanCoder<1>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<2> : public fixedHuffmanCoderBase<uint8_t, uint8_t>, public singleton<fixedHuffmanCoder<2>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint8_t, uint8_t>(2), singleton<fixedHuffmanCoder<2>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<3> : public fixedHuffmanCoderBase<uint8_t, uint8_t>, public singleton<fixedHuffmanCoder<3>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint8_t, uint8_t>(3), singleton<fixedHuffmanCoder<3>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<4> : public fixedHuffmanCoderBase<uint8_t, uint8_t>, public singleton<fixedHuffmanCoder<4>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint8_t, uint8_t>(4), singleton<fixedHuffmanCoder<4>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<5> : public fixedHuffmanCoderBase<uint16_t, uint8_t>, public singleton<fixedHuffmanCoder<5>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint16_t, uint8_t>(5), singleton<fixedHuffmanCoder<5>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<6> : public fixedHuffmanCoderBase<uint16_t, uint8_t>, public singleton<fixedHuffmanCoder<6>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint16_t, uint8_t>(6), singleton<fixedHuffmanCoder<6>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<7> : public fixedHuffmanCoderBase<uint16_t, uint8_t>, public singleton<fixedHuffmanCoder<7>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint16_t, uint8_t>(7), singleton<fixedHuffmanCoder<7>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <>
class fixedHuffmanCoder<8> : public fixedHuffmanCoderBase<uint16_t, uint8_t>, public singleton<fixedHuffmanCoder<8>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoderBase<uint16_t, uint8_t>(8), singleton<fixedHuffmanCoder<8>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};
}		// msdb
#endif	// _MSDB_FIXED_HUFFMAN_CODER_H_