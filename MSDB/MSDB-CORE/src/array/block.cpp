#include <array/block.h>

namespace msdb
{
block::block(pBlockDesc desc)
	: desc_(desc)
{

}
block::~block()
{

}
pBlockDesc block::getDesc()
{
	return this->desc_;
}
}
