#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_H_
#define _MSDB_DUMMY_ASTRONOMY_H_

#include <pch.h>

namespace msdb
{
namespace caDummy
{
using dim_type = position_t;

namespace data2D_star1024x1024
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type dimX = 1024;
static const dim_type dimY = 1024;
//static const dim_type dimX = 4096;
//static const dim_type dimY = 2048;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 2;
static const arrayId aid = 24241;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[dataLength];
	getDummy(data, dataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(data, aid, "data2D_star1024x1024", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_star1024x1024
}	// caDummy
}	// msdb
#endif	// _MSDB_DUMMY_ASTRONOMY_H_