#include <array/dimensionDesc.h>

namespace msdb
{
position_t msdb::dimensionDesc::getLength()
{
	return this->end_ - this->start_;
}
size_t dimensionDesc::getChunkNum()
{
	if(this->getLength() % this->chunkSize_)
	{
		return this->getLength() / this->chunkSize_ + 1;
	}
	return this->getLength() / this->chunkSize_;
}
std::vector<position_t> dimensionDescs::getDims()
{
	std::vector<position_t> dims;
	for(auto it = this->begin(); it != this->end(); it++)
	{
		dims.push_back((*it)->getLength());
	}

	return dims;
}
}