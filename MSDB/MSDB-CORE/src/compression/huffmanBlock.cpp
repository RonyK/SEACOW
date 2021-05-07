#include <stdafx.h>
#include <compression/huffmanBlock.h>

namespace msdb
{
huffmanBlock::huffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

huffmanBlock::~huffmanBlock()
{

}
}	// msdb