#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <util/coordinate.h>
#include <array/dimensionId.h>
#include <array/dimension.h>
#include <string>
#include <vector>

namespace msdb
{
class dimensionDesc;
using pDimensionDesc = std::shared_ptr<dimensionDesc>;

class dimensionDescs : public std::vector<pDimensionDesc>, public std::enable_shared_from_this<dimensionDescs>
{
public:
	std::vector<position_t> getDims();
	std::vector<position_t> getChunkDims();
	std::vector<position_t> getChunkContainerDims();
	std::vector<position_t> getBlockDims();
	std::vector<position_t> getBlockContainerDims();
};

class dimensionDesc
{
public:
	using dimension_type = position_t;

public:
	dimensionDesc(dimensionId id, std::string name, 
				  dimension_type start, dimension_type end,
				  position_t chunkSize, position_t blockSize = 0);

public:
	position_t getLength();
	size_t getChunkNum();

public:
	dimensionId id_;
	std::string name_;
	dimension_type start_;
	dimension_type end_;
	position_t chunkSize_;
	position_t blockSize_;
};
}

#endif