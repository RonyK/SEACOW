#include <pch.h>
#include "spihtDummy.h"

namespace msdb
{
	namespace caDummy
	{
		namespace data2D_spiht_sc4x4
		{
			std::vector<dim_type> dims = { dimY, dimX };
			std::vector<dim_type> chunkDims = { 4, 4 };
			std::vector<dim_type> chunkNums = { 1, 1 };

			void getWTDummy(value_type* output, size_t length)
			{
				const static value_type arr[][4] = {
					{26, 6, 13, 10},
					{-7, 7, 6, 4},
					{4, -4, 4, -3},
					{2, -2, -2, 0}
				};

				for (int y = 0; y < 4; y++)
				{
					for (int x = 0; x < 4; x++)
					{
						output[y * 4 + x] = arr[y][x];
					}
				}
			}
		}
	}
}