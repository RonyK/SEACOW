#include "pch.h"
#include "../TrialWavelet/compact.h"

namespace caWavelet
{
	void get2DDummy_s4x4(char*, size_t);
	void get2DWTDummy_s4x4(char*, size_t);

#define DATA_LENGTH_2D_4x4_DUMMY	16
	TEST(caCompact, d2_s4x4_compact)
	{
		using dim_type = unsigned char;
		using value_type = char;

		//////////////////////////////
		// Build Dummy Data
		char data[DATA_LENGTH_2D_4x4_DUMMY];
		char wtData[DATA_LENGTH_2D_4x4_DUMMY];

		get2DDummy_s4x4(data, DATA_LENGTH_2D_4x4_DUMMY);
		get2DWTDummy_s4x4(wtData, DATA_LENGTH_2D_4x4_DUMMY);

		std::vector<dim_type> chunkDims = { 2, 2 };
		std::vector<dim_type> chunkNums = { 2, 2 };
		std::vector<dim_type> dims = { 4, 4 };
		size_t maxLevel = 0;

		caMMT<dim_type, value_type> mmt;
		mmt.buildMMT(data, DATA_LENGTH_2D_4x4_DUMMY, &dims, &chunkDims, maxLevel);

		bstream bs;
		//////////////////////////////

		caCompact<dim_type, value_type> compact(dims, chunkNums, maxLevel);
		
		compact.encode(bs, wtData, mmt);
	}

	void get2DDummy_s4x4(char* output, size_t length)
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

	void get2DWTDummy_s4x4(char* output, size_t length)
	{
		assert(length == 4 * 4);
		const static char arr[4][4] = {
			{12, 8, 2, 2 },
			{8, 4, 6, 2 },
			{4, 0, 0, 2 },
			{2, 0, 0, 2 }
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