#include <stdafx.h>
#include <compression/lzwHuffmanBlock.h>

namespace msdb
{
lzwHuffmanBlock::lzwHuffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
lzwHuffmanBlock::~lzwHuffmanBlock()
{
}
}	// msdb