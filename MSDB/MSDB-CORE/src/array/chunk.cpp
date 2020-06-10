#include <array/chunk.h>

namespace msdb
{
chunk::chunk(chunkDesc* desc) : cached_(nullptr)
{
	this->desc_ = *desc;
}
bool chunk::isMaterialized() const
{
	if(this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
}
