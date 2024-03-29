#include <pch_test.h>
#include "dummy.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
inline char unsignedCharToSigned(char value)
{
	return value - 128;
}

namespace data2D_sc4x4
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 2, 2 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };

std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	const static value_type arr[][4] = {
		{9,7,6,2},
		{5,3,4,4},
		{8,2,4,0},
		{6,0,2,2}
	};

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			output[y * 4 + x] = arr[y][x];
		}
	}
}

void getChunkDummy(value_type* output, size_t length)
{
	const static value_type arr[][4] = {
		{9,7,5,3},
		{6,2,4,4},
		{8,2,6,0},
		{4,0,2,2}
	};

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			output[y * 4 + x] = arr[y][x];
		}
	}
}

void getWTDummy(value_type* output, size_t length)
{
	assert(length >= 4 * 4);
	// Wavelet represent array as a band-wise fashion
	const static value_type arr[4][2][2] = {
		{ {6, 4}, {4, 2}},
		{ {1, 1}, {3, 1}},
		{ {2, 0}, {1, 0}},
		{ {0, 1}, {0, 1}}
	};

	// In multi-dimensional fashion
	//{
	//	{6, 4, 1, 1 },
	//	{4, 2, 3, 1 },
	//	{2, 0, 0, 1 },
	//	{1, 0, 0, 1 }
	//};

	const value_type* seqArr = (value_type*)arr;

	for (int i = 0; i < 4 * 4; i++)
	{
		output[i] = seqArr[i];
	}
}

void getWTChunkDummy(value_type* output, size_t length)
{
	assert(length >= 4 * 4);
	// Wavelet represent array as a band-wise fashion
	const static value_type arr[dataLength] = {
		6, -2, -4, 0, 4, -2, 0, 4, 4, -6, -2, 0, 2, -2, 0, 4
	};
	//const static value_type arr[dataLength] = {
	//	6, 1, 2, 0, 4, 1, 0, 1, 4, 3, 1, 0, 2, 1, 0, 1
	//};

	// In multi-dimensional fashion
	//{
	//	{6, 4, 1, 1 },
	//	{4, 2, 3, 1 },
	//	{2, 0, 0, 1 },
	//	{1, 0, 0, 1 }
	//};

	const value_type* seqArr = (value_type*)arr;
	for (int i = 0; i < 4 * 4; i++)
	{
		output[i] = seqArr[i];
	}
}

void getExDummy(value_type* output, size_t length)
{
	assert(length > 10);
	// Wavelet represent array as a band-wise fashion
	const static value_type arr[9] = {
		0x64, 0x42, 0x28, 0x10, 0x4C, 0x40, 0x82
	};

	for (int i = 0; i < 9; i++)
	{
		output[i] = arr[i];
	}
}

void getExMMTBuilded(value_type(*minOutput)[2][2], value_type(*maxOutput)[2][2], size_t length)
{
	assert(length >= 2 * 2 * 2);

	const static value_type origianlMin[2][2][2] =
	{
		{{3, 2}, {0, 0}},
		{{0}}
	};

	const static value_type originalMax[2][2][2] =
	{
		{{9, 6}, {8, 4}},
		{{9}}
	};

	const static value_type min[2][2][2] =
	{
		{{2, 2}, {0, 0}},
		{{0}}
	};

	const static value_type max[2][2][2] =
	{
		{{9, 7}, {9, 7}},
		{{9}}
	};

	const static value_type bMin[2][2][2] =
	{
		{{2, 2}, {1, 1}},
		{{1}}
	};

	const static value_type bMax[2][2][2] =
	{
		{{4, 3}, {4, 3}},
		{{4}}
	};

	const static value_type bMinDelta[2][2][2] =
	{
		{{1, 1}, {0, 0}},
		{{0}}
	};


	const static value_type bMaxDelta[2][2][2] =
	{
		{{0, 1}, {0, 1}},
		{{0}}
	};

	const static value_type order[2][2][2] =
	{
		{{1, 1}, {1, 1}},
		{{1}}
	};

	memcpy(minOutput, min, sizeof(value_type) * 2 * 2 * 2);
	memcpy(maxOutput, max, sizeof(value_type) * 2 * 2 * 2);
}

void getExDelta(value_type* output, size_t length)
{
	assert(length >= 4 * 4);
	const static value_type arr[4][2][2] = {
		{ {7, 5}, {3, 1}},
		{ {4, 0}, {2, 2}},
		{ {8, 2}, {6, 0}},
		{ {4, 0}, {2, 2}}
	};

	// In multi-dimensional fashion
	//
	//	{7,5,4,0},
	//	{3,1,2,2},
	//	{8,2,4,0},
	//	{6,0,2,2}
	//};

	const value_type* seqArr = (value_type*)arr;
	for (int i = 0; i < 4 * 4; i++)
	{
		output[i] = seqArr[i];
	}
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
}	// data2D_sc4x4

namespace data2D_tempTest
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 1, 1 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };

std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	const static value_type arr[][4] = {
		{3, 3, 2, 2},
		{3, 3, 2, 4},
		{4, 14, 13, 4},
		{4, 2, 3, 12}
	};

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			output[y * 4 + x] = arr[y][x];
		}
	}
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
}	// data2D_tempTest

namespace data2D_test32x32
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 1, 1 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = { 1, 1 };
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };

void getDummy(value_type* output, size_t length)
{
	const static value_type arr[][dimX] = {
		{2, 0, 0, 0, 0, 0, 0, 0, 32, -40, 65, -14, 34, 2, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, 112, -30, 65, -14, 34, 2, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, -48, -31, 65, -14, 34, 2, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, -64, -35, 65, -14, 34, 2, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, -128, -31, 65, -14, 34, 2, 0, 0},
		{-3, -3 , -3 , -3, 0, 0, 0, 0, -3, -10, 34, 79, 0, 19, 0, -128},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, -111, -10, 54, 79, 0, 20, 0, -128}
	};

	for (int y = 0; y < dimY; y++)
	{
		for (int x = 0; x < dimX; x++)
		{
			output[y * dimX + x] = arr[y][x];
		}
	}
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
}

namespace data2D_sc8x8
{
std::vector<dim_type> dims = { 8, 8 };
std::vector<dim_type> originalDims = { dimY, dimX };
std::vector<dim_type> chunkDims = { 2, 2 };
std::vector<dim_type> chunkNums = { 4, 4 };
std::vector<dim_type> blockDims = { 2, 2 };

void getDummy(value_type* output, size_t length)
{
	const static value_type arr[dimY][dimX] = {
		{105, 91, 94, 66,		6, 26, 52, 40	},
		{115, 101, 74, 50,		4, 32, -40, -56	},

		{108, 100, 75, 71,		24, 12, -1, 3	},
		{100, 96, 79, 67,		30, 18, -3, -3	},

		{115, 103, 93, 103,		62, 54, 54, 38	},
		{109, 101, 95, 113,		48, 44, 64, 44	},

		{118, 110, 105, 107,	63, 59, 63, 57	},
		{108, 100, 81, 87,		55, 47, 49, 47	}
	};

	for (int y = 0; y < dimY; y++)
	{
		for (int x = 0; x < dimX; x++)
		{
			output[y * dimX + x] = arr[y][x];
		}
	}
}

void getWTDummy(value_type* output, size_t length)
{
	// TODO::
	//assert(length == dimY * dimX);
	//// Wavelet represent array as a band-wise fashion
	//const static value_type arr[dimY][dimY / 2][dimX / 2] = {
	//	{ {6, 4}, {4, 2}},
	//	{ {1, 1}, {3, 1}},
	//	{ {2, 0}, {1, 0}},
	//	{ {0, 1}, {0, 1}}
	//};

	// In multi-dimensional fashion
	//{
	//	{6, 4, 1, 1 },
	//	{4, 2, 3, 1 },
	//	{2, 0, 0, 1 },
	//	{1, 0, 0, 1 }
	//};

	//const value_type* seqArr = (value_type*)arr;

	//for (int i = 0; i < dimY * dimX; i++)
	//{
	//	output[i] = seqArr[i];
	//}
}

void getExMMTBuilded(value_type(*minOutput)[4][4], value_type(*maxOutput)[4][4], size_t length)
{
	assert(length >= 48);

	const static value_type min[3][4][4] = {
		{
			{91, 50, 4, -56},
			{96, 67, 12, -3},
			{101, 93, 44, 38},
			{100, 81, 47, 47}
		},
		{
			{50, -56},
			{81, 38}
		},
		{
			{-56}
		}
	};

	const static value_type max[3][4][4] = {
		{
			{115, 94, 32, 52},
			{108, 79, 30, 3},
			{115, 113, 62, 64},
			{118, 107, 63, 63}
		},
		{
			{115, 52},
			{118, 64}
		},
		{
			{118}
		}
	};

	memcpy(minOutput, min, sizeof(value_type) * 3 * 4 * 4);
	memcpy(maxOutput, max, sizeof(value_type) * 3 * 4 * 4);
}

void getExSerialMMT(value_type* output, size_t length)
{
	assert(length > 20);
	const static value_type arr[20] = {
		0x76, 0xC8, 0x0C, 0x10, 0x0D,
		0x0C, 0x40, 0x90, 0x05, 0x1A,
		0x44, 0x00, 0x18, 0x00, 0x1A
	};

	for (size_t i = 0; i < 20; i++)
	{
		output[i] = arr[i];
	}
}

void getExDummy(value_type* output, size_t length)
{
	// TODO :: Implement getExDummy()
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}
}		// caDummy
}		// core
}		// msdb