#include <pch.h>
#include <dummy/dummy_astronomy.h>
#include <fstream>
#include <string>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 8, 8 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	assert(length >= dims[0] * dims[1]);
	std::ifstream in("STSCI-H-p1942a-f-1024x1024_Gray.txt");
	//std::ifstream in("Mars_Viking_MDIM21_ClrMosaic_1km_4096x2048_gray.txt");
	assert(in.is_open() == true);

	int c;
	int i = 0;
	while(in && i < dataLength)
	{
		in >> c;
		output[i++] = c / 2;
	}
	in.close();
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr)
{
	sourceArr = getSourceArrayDesc();
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}	// data2D_star1024x1024s

namespace data2D_saturn1024x1024
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 8, 8 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	assert(length >= dims[0] * dims[1]);
	std::ifstream in("STSCI-H-p1936a-f-1024x1024_gray.txt");
	assert(in.is_open() == true);

	int c;
	int i = 0;
	while (in && i < dataLength)
	{
		in >> c;
		output[i++] = c / 2;
	}
	in.close();
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr)
{
	sourceArr = getSourceArrayDesc();
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 8, 8 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	assert(length >= dims[0] * dims[1]);
	std::ifstream in("latest10240171copy3_1024x1024_gray.txt");
	assert(in.is_open() == true);

	int c;
	int i = 0;
	while (in && i < dataLength)
	{
		in >> c;
		output[i++] = c / 2;
	}
	in.close();
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr)
{
	sourceArr = getSourceArrayDesc();
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 16, 8 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	assert(length >= dims[0] * dims[1]);
	std::ifstream in("Mars_Viking_MDIM21_ClrMosaic_1km_4096x2048_gray.txt");
	assert(in.is_open() == true);

	int64_t c;
	int64_t i = 0;
	while (in && i < length)
	{
		in >> c;
		output[i++] = c / 2;
	}
	in.close();
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr)
{
	sourceArr = getSourceArrayDesc();
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}	// data2D_mars4096x2048
}	// atDummy
}	// msdb



