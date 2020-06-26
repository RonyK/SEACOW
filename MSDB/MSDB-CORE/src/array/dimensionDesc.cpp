#include <array/dimensionDesc.h>
#include <util/math.h>

namespace msdb
{
dimensionDesc::dimensionDesc(dimensionId id, std::string name,
							 dimension_type start, dimension_type end, 
							 position_t chunkSize)
	: id_(id), name_(name), start_(start), end_(end), chunkSize_(chunkSize)
{

}

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
std::vector<position_t> dimensionDescs::getChunkDims()
{
	std::vector<position_t> dims;
	for (auto it = this->begin(); it != this->end(); it++)
	{
		dims.push_back((*it)->chunkSize_);
	}

	return dims;
}

std::vector<position_t> dimensionDescs::getChunkContainerDims()
{
	std::vector<position_t> dims;
	for (auto it = this->begin(); it != this->end(); it++)
	{
		dims.push_back(intDivCeil((*it)->getLength(), (*it)->chunkSize_));
	}

	return dims;
}

}	// msdb