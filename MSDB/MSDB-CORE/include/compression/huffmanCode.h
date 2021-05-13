#pragma once
#ifndef _MSDB_HUFFMAN_CODE_H_
#define _MSDB_HUFFMAN_CODE_H_

#include <stdafx.h>
#include <io/bitstream.h>
#include <stack>
#include <queue>

namespace msdb
{
template <typename codeType, typename symbolType>
class huffmanCoder
{
public:
	using codeLenType = unsigned char;

	static const unsigned char maxTableLevel = 16;
	static const unsigned char bitSymbol = sizeof(symbolType) * CHAR_BIT;
	static const unsigned char bitLevelUnit = 4;
	inline static const uint16_t remainMask[5] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F };
	static const size_t maxLevel = sizeof(codeType) * CHAR_BIT / bitLevelUnit;

public:
	huffmanCoder()
		: bits_(sizeof(symbolType) * CHAR_BIT)
	{
	}

	huffmanCoder(size_t bits)
		: bits_(bits)
	{
	}

	~huffmanCoder()
	{
		if (this->root_)
		{
			this->deleteTree();
		}
	}

private:
	struct huffmanNode
	{
		huffmanNode()
			: code_(0), symbol_(0), codeLen_(0), parent_(nullptr), left_(nullptr), right_(nullptr)
		{
			this->initTableCodeMask();
		}
		
		huffmanNode(symbolType symbol)
			: huffmanNode(), symbol_(symbol)
		{
		}

		huffmanNode(huffmanNode* left, huffmanNode* right)
			: huffmanNode(), left_(left), right_(right)
		{
		}

		huffmanNode(codeType code, symbolType symbol, codeLenType codeLen, huffmanNode* parent = nullptr, huffmanNode* left = nullptr, huffmanNode* right = nullptr)
			: code_(code), symbol_(symbol), codeLen_(codeLen), parent_(parent), left_(left), right_(right)
		{
			this->initTableCodeMask();
		}

		bool isLeaf()
		{
			return !this->left_ && !this->right_;
		}

		codeType code_;
		symbolType symbol_;
		codeLenType codeLen_;
		huffmanNode* left_;
		huffmanNode* right_;
		huffmanNode* parent_;
	};

	struct compareNode
	{
		bool operator()(std::pair<size_t, huffmanNode*> a, std::pair<size_t, huffmanNode*> b)
		{
			return a.first > b.first;
		}
	};

	struct decodeTableNode
	{
		decodeTableNode()
			: isLeaf_(false), node_(nullptr)
		{

		}

		decodeTableNode(huffmanNode node)
			: node_(node)
		{
			if (node)
			{
				this->isLeaf_ = true;
			} else
			{
				this->isLeaf_ = false;
			}
		}
		bool isLeaf_;
		huffmanNode* node_;
		std::vector<decodeTableNode> childLevel_;
	};

private:
	codeType tableCodeMask[maxTableLevel];

	void initTableCodeMask()
	{
		uint64_t mask = this->remainMask[this->bitLevelUnit];		// 1111
		for(int i = 0; i < maxTableLevel; ++i)
		{
			this->tableCodeMask[i] = mask;
			mask <<= this->bitLevelUnit;
		}
	}

	void buildTree(std::vector<size_t>& freq)
	{
		std::priority_queue<std::pair<size_t, huffmanNode*>, compareNode> qFreq;
		for(size_t i = 0; i < freq.size(); ++i)
		{
			if (freq[i])
			{
				qFreq.push({ freq[i], new huffmanNode(i) });
			}
		}

		size_t rootFreq = 0;
		{
			// Build root node
			assert(qFreq.size() >= 2);

			auto left = qFreq.top();
			qFreq.pop();
			auto right = qFreq.top();
			qFreq.pop();
			rootFreq = left.first + right.first;

			this->root_ = new huffmanNode(left.second, right.second);
		}

		while (!qFreq.empty())
		{
			auto next = qFreq.top();
			qFreq.pop();

			huffmanNode* left = nullptr, right = nullptr;

			if (rootFreq < next.first)
			{
				left = this->root_;
				right = this->next.second;
			} else
			{
				left = this->next.second;
				right = this->root_;
			}

			this->root_ = new huffmanNode(left, right);

			left->parent_ = this->root_;
			right->parent_ = this->root_;

			rootFreq += next.first;
		}
	}

	void deleteTree()
	{
		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		while (!dfsStack.empty())
		{
			auto node = dfsStack.top();
			dfsStack.pop();

			if (node->left_)
			{
				dfsStack.push(node->left_);
			}
			if(node->right_)
			{
				dfsStack.push(node->right_);
			}

			delete node;
		}
	}

public:
	void encode(bstream& out, symbolType* in, size_t len)
	{
		std::vector<size_t> freq(std::numeric_limits<symbolType>::max(), 0);
		for(size_t i = 0; i < len; ++i)
		{
			++freq[in[i]];
		}

		buildTree(freq);
		encodeTree(out);

		for(size_t i = 0; i < len; ++i)
		{
			encodeSymbol(out, in[i]);
		}
	}

	void encodeTree(bstream& out)
	{
		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		while (!dfsStack.empty())
		{
			huffmanNode* node = dfsStack.top();
			dfsStack.pop();
			
			assert(node != nullptr);

			if (node->isLeaf())
			{
				out << setw(1) << (unsigned char)0x1;
				out << setw(bitSymbol) << node->symbol_;
			} else
			{
				out << setw(1) << (unsigned char)0x0;

				dfsStack.push(node->left_);
				dfsStack.push(node->right_);
			}
		}
	}

	void encodeSymbol(bstream& out, symbolType symbol)
	{
		// TODO:: output encode table or tree
#ifndef NDEBUG
		assert(this->encodeLookupTable_.find(symbol) != this->encodeLookupTable_.end());
#endif

		auto node = this->encodeLookupTable_[symbol];
		out << setw(node->codeLen_) << node->code;
	}

	void decode(symbolType* out, size_t lenOut, bstream& in)
	{
		this->decodeTree(in);

		// TODO:: build decode table

		codeType code = 0x0;
		in >> setw(sizeof(symbolType) * CHAR_BIT) >> code;
		for(size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code, in);
			out[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(sizeof(symbolType) * CHAR_BIT - result.second) >> nextCode;
			code = (code << result.second) | nextCode;
			
		}
	}

	void decodeTree(bstream& in)
	{
		this->deleteTree();

		this->root_ = new huffmanNode();

		std::stack<codeType> dfsStack;
		char isLeaf = 0;
		bool isLeaf = false;
		codeType code = 0;
		dfsStack.push(0);
		while(!dfsStack.empty())
		{
			code = dfsStack.top();
			dfsStack.pop();

			in >> setw(1) >> isLeaf;
			if(isLeaf)
			{
				symbolType symbol;
				in >> setw(bitSymbol) >> symbol;

			}else
			{
				dfsStack.push(code << 1);
				dfsStack.push(code << 1 | (codeType)0x1);
			}
		}
	}

	std::pair<symbolType, size_t> decodeSymbol(codeType code)
	{
		codeType code = 0;
		size_t remain = bitLevelUnit;
		size_t maxLevel = sizeof(symbolType) * CHAR_BIT / bitLevelUnit;

		for (size_t level = 0; level < maxLevel; ++level)
		{
			//code = code & remainMask[remain];
			//code <<= remain;
			//in >> setw(remain) >> code;

			//auto node = this->decodeLookupTable[level][code];
			//assert(node != nullptr);
			//if (node->isLeaf_)
			//{
			//	return { node->symbol_, node->codeLen_ };
			//}
		}

	}

private:
	size_t bits_;
	std::map<symbolType, huffmanNode*> encodeLookupTable_;
	std::vector<decodeTableNode*> decodeLookupTable_;
	huffmanNode* root_;
};
}		// msdb
#endif	// _MSDB_HUFFMAN_CODE_H_