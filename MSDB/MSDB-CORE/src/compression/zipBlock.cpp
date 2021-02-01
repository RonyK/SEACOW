#include <stdafx.h>
#include <compression/zipBlock.h>

namespace msdb
{
zipBlock::zipBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

zipBlock::~zipBlock()
{
}
}	// msdb