#include <array/dimensionDesc.h>
#include <util/math.h>

namespace msdb
{
dimensionDesc::dimensionDesc(dimensionId id, std::string name,
							 dimension_type start, dimension_type end, 
							 position_t chunkSize, position_t blockSize)
	: id_(id), name_(name), start_(start), end_(end), 
	chunkSize_(chunkSize), blockSize_(blockSize)
{
}

dimensionDesc::dimensionDesc(const dimensionDesc& mit)
	: id_(mit.id_), name_(mit.name_), start_(mit.start_), end_(mit.end_),
	chunkSize_(mit.chunkSize_), blockSize_(mit.blockSize_)
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

dimension dimensionDescs::getBlockDims()
{
	dimension blockDims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		blockDims[d] = this->at(d)->blockSize_;
	}
	return blockDims;
}

dimension dimensionDescs::getBlockSpace()
{
	return dimension(this->getChunkDims()) / this->getBlockDims();
}

//dimension dimensionDescs::getBlockContainerDims()
//{
//	return std::vector<position_t>();
//}
}	// msdb