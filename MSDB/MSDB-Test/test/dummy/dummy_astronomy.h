#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_H_
#define _MSDB_DUMMY_ASTRONOMY_H_

#include <pch.h>

namespace msdb
{
using dim_type = position_t;

namespace caDummy
{
void getDummyFromBinaryFile(char* output, const char* filePath, size_t length, size_t sizeofValueType);

namespace data2D_star1024x1024
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type origianlDimX = 1024;
static const dim_type originalDimY = 1024;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const dim_type dimX = 1024;
static const dim_type dimY = 1024;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 3;
static const arrayId aid = 19420;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_star1024x1024", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_star1024x1024", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type origianlDimX = 1024;
static const dim_type originalDimY = 1024;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const dim_type dimX = 1024;
static const dim_type dimY = 1024;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 3;
static const arrayId aid = 19360;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_saturn1024x1024", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_saturn1024x1024", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type origianlDimX = 1024;
static const dim_type originalDimY = 1024;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const dim_type dimX = 1024;
static const dim_type dimY = 1024;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 3;
static const arrayId aid = 10240;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_solar1024x1024", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_solar1024x1024", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type origianlDimX = 4096;
static const dim_type originalDimY = 2048;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const dim_type dimX = 4096;
static const dim_type dimY = 2048;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 3;
static const arrayId aid = 40960;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_mars4096x2048", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_mars4096x2048", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
using value_type = char;
static const eleType ele_type = eleType::CHAR;

static const dim_type origianlDimX = 20480;
static const dim_type originalDimY = 10240;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const dim_type dimX = 16384;
static const dim_type dimY = 8192;
static const size_t dataLength = dimX * dimY;
static const size_t wtLevel = 3;
static const size_t mmtLevel = 3;
static const arrayId aid = 20100;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_mercurydem20480x10240", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_mercurydem20480x10240", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb
#endif	// _MSDB_DUMMY_ASTRONOMY_H_