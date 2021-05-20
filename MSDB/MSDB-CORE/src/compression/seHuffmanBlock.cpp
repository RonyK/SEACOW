#include <stdafx.h>
#include <compression/seHuffmanBlock.h>

namespace msdb
{
seHuffmanBlock::seHuffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
seHuffmanBlock::~seHuffmanBlock()
{
}
}	// msdb