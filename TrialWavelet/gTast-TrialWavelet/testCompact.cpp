#include "pch.h"
#include "../TrialWavelet/compact.h"

namespace caWavelet
{
	void get2DDummy_s4x4(char*, size_t);
	void get2DWTDummy_s4x4(char*, size_t);
	void getEx2DWTDummy_s4x4(char*, size_t );

#define DATA_LENGTH_2D_4x4_DUMMY	16
	TEST(caCompact, d2_s4x4_compact)
	{
		using dim_type = unsigned char;
		using value_type = char;

		//////////////////////////////
		// Build Dummy Data
		char data[DATA_LENGTH_2D_4x4_DUMMY];
		char wtData[DATA_LENGTH_2D_4x4_DUMMY];
		char expected[DATA_LENGTH_2D_4x4_DUMMY];

		get2DDummy_s4x4(data, DATA_LENGTH_2D_4x4_DUMMY);
		get2DWTDummy_s4x4(wtData, DATA_LENGTH_2D_4x4_DUMMY);
		getEx2DWTDummy_s4x4(expected, DATA_LENGTH_2D_4x4_DUMMY);

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

		const char* converted = bs.c_str();
		for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
		{
			std::cout << std::dec << "[" << i << "]" << std::setfill('0') << std::setw(2) << std::right << std::hex << (short)converted[i] << std::endl;
			EXPECT_EQ((unsigned char)expected[i], (unsigned char)converted[i]);
		}
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

	void getEx2DWTDummy_s4x4(char* output, size_t length)
	{
		assert(length > 10);
		// Wavelet represent array as a band-wise fashion
		const static char arr[9] = {
			6, 4, 4, 2, 0x28, 0x22, 0x62, 0x04, 0x10
		};

		for (int i = 0; i < 9; i++)
		{
			output[i] = arr[i];
		}
	}
}