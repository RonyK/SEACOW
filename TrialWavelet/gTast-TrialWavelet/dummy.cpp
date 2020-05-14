#include "pch.h"
#include "dummy.h"

namespace caWavelet
{
	namespace caDummy
	{
		namespace data2D_s4x4
		{
			const size_t dataLength = 16;
			const size_t dimX = 4;
			const size_t dimY = 4;

			std::vector<dim_type> chunkDims = { 2, 2 };
			std::vector<dim_type> chunkNums = { 2, 2 };
			std::vector<dim_type> dims = { 4, 4 };
			size_t maxLevel = 0;

			void getDummy(char* output, size_t length)
			{
				const static char arr[][4] = {
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

			void getWTDummy(char* output, size_t length)
			{
				assert(length >= 4 * 4);
				// Wavelet represent array as a band-wise fashion
				const static char arr[4][2][2] = {
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

				const char* seqArr = (char*)arr;

				for (int i = 0; i < 4 * 4; i++)
				{
					output[i] = seqArr[i];
				}
			}

			void getEXDummy(char* output, size_t length)
			{
				assert(length > 10);
				// Wavelet represent array as a band-wise fashion
				const static char arr[9] = {
					0x64, 0x42, 0x28, 0x10, 0x4C, 0x40, 0x82
				};

				for (int i = 0; i < 9; i++)
				{
					output[i] = arr[i];
				}
			}

		}

		namespace data2D_s8x8
		{
			const size_t dim_x = 8;
			const size_t dim_y = 8;

			void getDummy(char* output, size_t length)
			{
				const static char arr[dim_y][dim_x] = {
					{105, 91, 94, 66,		6, 26, 52, 40	},
					{115, 101, 74, 50,		4, 32, -40, -56	},
					{108, 100, 75, 71,		24, 12, -1, 3	},
					{100, 96, 79, 67,		30, 18, -3, -3	},
					{115, 103, 93, 103,		62, 54, 54, 38	},
					{109, 101, 95, 113,		48, 44, 64, 44	},
					{118, 110, 105, 107,	63, 59, 63, 57	},
					{108, 100, 81, 87,		55, 47, 49, 47	}
				};

				for (int y = 0; y < dim_y; y++)
				{
					for (int x = 0; x < dim_x; x++)
					{
						output[y * dim_x + x] = arr[y][x];
					}
				}
			}

			void getWTDummy(char* output, size_t length)
			{
				assert(length == dim_y * dim_x);
				// Wavelet represent array as a band-wise fashion
				const static char arr[dim_y][dim_y / 2][dim_x / 2] = {
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

				const char* seqArr = (char*)arr;

				for (int i = 0; i < dim_y * dim_x; i++)
				{
					output[i] = seqArr[i];
				}
			}

			void getExDummy(char* output, size_t length)
			{
				assert(length > 10);
				// Wavelet represent array as a band-wise fashion
				const static char arr[9] = {
					0x64, 0x42, 0x28, 0x10, 0x4C, 0x40, 0x82
				};

				for (int i = 0; i < 9; i++)
				{
					output[i] = arr[i];
				}
			}
		}
	}
}