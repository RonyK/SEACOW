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
std::vector<dim_type> originalDims = { dimY, dimX };
//std::vector<dim_type> chunkNums = { 1, 1 };
std::vector<dim_type> chunkNums = { 16, 16 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
//std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_STAR_STSCI-H-p1942a-f-1024x1024_gray.txt",
		length, sizeof(value_type));
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
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 16, 16 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_SATURN_STSCI-H-p1936a-f-1024x1024_gray.txt",
		length, sizeof(value_type));
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
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 16, 16 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_SOLAR_latest10240171copy3_1024x1024_gray.txt",
		length, sizeof(value_type));
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
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 16, 32 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_MARS_Viking_MDIM21_ClrMosaic_1km_4096x2048_gray.txt",
		length, sizeof(value_type));
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

namespace data2D_mercurydem20480x10240
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> originalDims = { dimY, dimX };
//std::vector<dim_type> chunkNums = { 1, 1 };
//std::vector<dim_type> chunkNums = { 8, 16};
//std::vector<dim_type> chunkNums = { 32, 64 };
std::vector<dim_type> chunkNums = { 64, 128 };
//std::vector<dim_type> chunkNums = { 128, 256 };
//std::vector<dim_type> chunkNums = { 256, 512};
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
//std::vector<dim_type> blockNums = { 8, 8 };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_Mercury_Messenger_DEM_Global_665m_v2_max_23040x11520.txt",
		length, sizeof(value_type));
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
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 64, 192 };
//std::vector<dim_type> chunkNums = { 16, 48 };
//std::vector<dim_type> chunkNums = { 128, 384 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
//std::vector<dim_type> blockNums = { 8, 8 };
//std::vector<dim_type> blockNums = { 4, 4 };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	getDummyFromBinaryFile(
		reinterpret_cast<char*>(output),
		"BIN_Lunar_LRO_WAC_GLD100_DTM_79S79N_100m_v1_109165x47912.txt",
		length, sizeof(value_type));
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
}	// data2D_lunar102400x40960

void getDummyFromBinaryFile(char* output, const char* filePath, size_t length, size_t sizeofValueType)
{
	std::ifstream in(filePath, std::ios::binary);
	assert(in.is_open() == true && "Check file is opened");

	// Get file size
	in.seekg(0, std::ios::end);
	size_t fileLength = (size_t)in.tellg();
	in.seekg(0, std::ios::beg);
	//assert(sizeofValueType * length == fileLength && "Data length and file length check");

	in.read(output, std::min({ length, fileLength }));
	in.close();
}
}	// atDummy
}	// msdb



