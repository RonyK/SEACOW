#include <pch.h>
#include "dummy.h"

namespace msdb
{
	namespace caDummy
	{
		namespace data2D_sc4x4
		{
			std::vector<dim_type> dims = { dimY, dimX };
			std::vector<dim_type> chunkDims = { 2, 2 };
			std::vector<dim_type> chunkNums = { 2, 2 };

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

		}

		namespace data2D_sc8x8
		{
			std::vector<dim_type> dims = { 8, 8 };
			std::vector<dim_type> chunkDims = { 2, 2 };
			std::vector<dim_type> chunkNums = { 4, 4 };

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
				assert(length == dimY * dimX);
				// Wavelet represent array as a band-wise fashion
				const static value_type arr[dimY][dimY / 2][dimX / 2] = {
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

				for (int i = 0; i < dimY * dimX; i++)
				{
					output[i] = seqArr[i];
				}
			}

			void getExMMT(value_type* output, size_t length)
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
		}

		namespace data2D_si8x8
		{
			std::vector<dim_type> dims = { 8, 8 };
			std::vector<dim_type> chunkDims = { 2, 2 };
			std::vector<dim_type> chunkNums = { 4, 4 };

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
				assert(length == dimY * dimX);
				// Wavelet represent array as a band-wise fashion
				const static value_type arr[dimY][dimY / 2][dimX / 2] = {
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

				for (int i = 0; i < dimY * dimX; i++)
				{
					output[i] = seqArr[i];
				}
			}

			void getExMMT(value_type* output, size_t length)
			{
				assert(length > 21);
				const static value_type arr[21] = {
					0x00, 0x00, 0x00, 0x76, 0xFF,
					0xFF, 0xFF, 0xC8, 0x0C, 0x10,
					0x0D, 0x0C, 0x40, 0x90, 0x05,
					0x1A, 0x44, 0x18, 0x58, 0x18,
					0x5A
				};

				for (size_t i = 0; i < 21; i++)
				{
					output[i] = arr[i];
				}
			}

			void getExDummy(value_type* output, size_t length)
			{
				// TODO :: Implement getExDummy()
			}
		}
	}
}