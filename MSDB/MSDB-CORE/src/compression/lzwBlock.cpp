#include <stdafx.h>
#include <compression/lzwBlock.h>
#include <util/ioutil.h>

namespace msdb
{
lzwBlock::lzwBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
lzwBlock::~lzwBlock()
{
}
}