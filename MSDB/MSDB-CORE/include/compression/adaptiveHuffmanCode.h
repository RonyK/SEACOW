#pragma once
#ifndef _MSDB_ADAPTIVE_HUFFMAN_CODING_H_
#define _MSDB_ADAPTIVE_HUFFMAN_CODING_H_

#include <stdafx.h>
#include <io/bitstream.h>

namespace msdb
{
template <typename codeType>
class aHuffmanCoder
{
public:
	using pathType = std::pair<codeType, size_t>;

public:
	aHuffmanCoder(size_t bits)
		: bits_(bits), nytNode(nullptr), root(new huffmanNode(-1, 0, pow(2, sizeof(codeType) * CHAR_BIT + 1), nullptr, nullptr, nullptr, true))
	{
		assert(sizeof(codeType) * CHAR_BIT >= bits);
		nytNode = this->root;
	}
	~aHuffmanCoder()
	{
		this->deleteTree(this->root);
	}

public:
	void updateTreeModel(codeType symbol)
	{
		huffmanNode* leftToIncrement = nullptr;
		huffmanNode* symbolNode = this->getSymbolNode(symbol, this->root);

		if (symbolNode == nullptr)
		{
			nytNode->isNYT = false;
			auto curMinOrder = nytNode->order;
			nytNode->left = new huffmanNode(-1, 0, curMinOrder - 2, nullptr, nullptr, nytNode, true);
			nytNode->right = new huffmanNode(symbol, 0, curMinOrder - 1, nullptr, nullptr, nytNode, false);
			symbolNode = nytNode->right;
			nytNode = nytNode->left;
		}

		this->slideAndIncrement(symbolNode);
	}

	void encode(bstream& out, codeType symbol)
	{
		huffmanNode* symbolNode = this->getSymbolNode(symbol, root);
		if (symbolNode)
		{
			auto result = this->getPathToSymbol(root, symbolNode, this->makePath(0, 0));
			out << setw(result.second) << result.first;
			this->updateTreeModel(symbol);
			return;
		}

		auto result = getPathToSymbol(root, nytNode, this->makePath(0, 0));
		this->updateTreeModel(symbol);

		if (result.second)
		{
			out << setw(result.second) << result.first;
		}
		out << setw(this->bits_) << symbol;
	}

	codeType decode(bstream& in)
	{
		codeType result = (codeType)-1;
		huffmanNode* cur = this->root;
		while (result == (codeType)-1)
		{
			if (cur->value != (codeType)-1)
			{
				result = cur->value;
				this->updateTreeModel(cur->value);
				cur = root;
			} else if (cur->isNYT)
			{
				result = 0;
				in >> setw(this->bits_) >> result;
				this->updateTreeModel(result);
				cur = root;
			} else
			{
				char b;
				in >> setw(1) >> b;

				if (b == 0)
				{
					cur = cur->left;
				} else
				{
					cur = cur->right;
				}
			}
		}

		return result;
	}

private:
	struct huffmanNode
	{
		huffmanNode()
			: value(0), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr), isNYT(false)
		{
		}

		huffmanNode(codeType value, size_t weight, size_t order, huffmanNode* left, huffmanNode* right,
					huffmanNode* parent, bool isNYT = false)
			:  value(value), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT)
		{
		}

		huffmanNode(bool isNYT, codeType value)
			: huffmanNode()
		{
			this->isNYT = isNYT;
			this->value = value;
		}

		codeType value;
		size_t weight;
		size_t order;
		huffmanNode* left;
		huffmanNode* right;
		huffmanNode* parent;
		bool isNYT;

	};
	huffmanNode* getSymbolNode(codeType symbol, huffmanNode* cur) const
	{
		if (cur == nullptr || cur->value == symbol)
		{
			return cur;
		}

		huffmanNode* leftResult = this->getSymbolNode(symbol, cur->left);
		if (leftResult != nullptr)
		{
			return leftResult;
		}

		return this->getSymbolNode(symbol, cur->right);
	}
	void findBlockLeader(huffmanNode* cur, huffmanNode*& curMax) const
	{
		if (cur == nullptr || curMax == root)
			return;

		if (cur->weight == curMax->weight && cur != curMax->parent && cur->order > curMax->order)
			curMax = cur;

		findBlockLeader(cur->left, curMax);
		findBlockLeader(cur->right, curMax);
	}

	void swapNodes(huffmanNode* first, huffmanNode* second)
	{
		if (first->parent == nullptr || second->parent == nullptr)
			return;

		if (first->parent == second || second->parent == first)
			return;

		huffmanNode*& firstRef = first->parent->left == first ? first->parent->left : first->parent->right;
		huffmanNode*& secondRef = second->parent->left == second ? second->parent->left : second->parent->right;

		std::swap(firstRef, secondRef);
		std::swap(firstRef->parent, secondRef->parent);
		std::swap(firstRef->order, secondRef->order);
	}

	std::pair<codeType, size_t> getPathToSymbol(huffmanNode* cur, huffmanNode* result, pathType curPath) const
	{
		if (cur == result)
		{
			return curPath;
		}

		if (cur == nullptr)
		{
			return std::make_pair<codeType, size_t>(0, 0);
		}

		auto left = this->getPathToSymbol(cur->left, result, this->makePath((curPath.first << 1), curPath.second + 1));
		if (left.second != 0)
		{
			return left;
		}

		return this->getPathToSymbol(cur->right, result, this->makePath((curPath.first << 1) | 1, curPath.second + 1));
	}

	void slideAndIncrement(huffmanNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		huffmanNode* blockLeader = node;
		this->findBlockLeader(this->root, blockLeader);
		if (blockLeader != node)
		{
			this->swapNodes(blockLeader, node);
		}

		++(node->weight);
		this->slideAndIncrement(node->parent);
	}

	void deleteTree(huffmanNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		this->deleteTree(node->left);
		this->deleteTree(node->right);

		delete node;
	}

	aHuffmanCoder::pathType makePath(codeType symbol, size_t depth) const
	{
		assert(depth < sizeof(codeType)* CHAR_BIT);
		return { symbol, depth };
	}

	huffmanNode* nytNode;
	huffmanNode* root;
	size_t bits_;
};
}		// msdb
#endif	// _MSDB_ADAPTIVE_HUFFMAN_CODING_H_