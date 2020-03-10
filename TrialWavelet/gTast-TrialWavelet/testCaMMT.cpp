#include "pch.h"
#include "../TrialWavelet/mmt.h"

namespace caWavelet
{
#define DATA_LENGTH_2D_DUMMY	64

	void build2DDummy(int* output, size_t length);
	void build2DDummy_s8x8(int* output, size_t length);
	void expected2DDummy_s8x8(int* output, size_t length);

	TEST(caMMT, buildLeafMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		int data[DATA_LENGTH_2D_DUMMY];
		build2DDummy(data, DATA_LENGTH_2D_DUMMY);
		caMMT<unsigned int, int> mmt;
		mmt.length = DATA_LENGTH_2D_DUMMY;
		mmt.dims_ = std::vector<unsigned int>() = { 8, 8 };
		std::vector<unsigned int> chunkDim = { 2, 2 };
		std::vector<unsigned int> chunkNum = { 4, 4 };

		std::vector<int> expectedMax = {
			9, 11, 13, 15,
			25, 27, 29, 31,
			41, 43, 45, 47,
			57, 59, 61, 63
		};
		std::vector<int> expectedMin = {
			0, 2, 4, 6,
			16, 18, 20, 22,
			32, 34, 36, 38,
			48, 50, 52, 54
		};
		//////////////////////////////

		mmt.buildLeafMMT(data, DATA_LENGTH_2D_DUMMY, &chunkDim, &chunkNum);

		for (size_t i = 0; i < chunkNum[0] * chunkNum[1]; i++)
		{
			EXPECT_EQ(mmt.nodes[0][i].max, expectedMax[i]);
			EXPECT_EQ(mmt.nodes[0][i].min, expectedMin[i]);
		}
	}

	TEST(caMMT, buildIntermediateMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		caMMT<unsigned int, int> mmt;
		size_t level = 1;
		mmt.length = DATA_LENGTH_2D_DUMMY;
		mmt.dims_ = std::vector<unsigned int>() = { 8, 8 };

		caMMT<unsigned int, int>::mmtNode* nodeL0 = new caMMT<unsigned int, int>::mmtNode[16];
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				caMMT<unsigned int, int>::mmtNode* cur = nodeL0 + x + 4 * y;
				cur->max = 2 * x + y * 16 + 9;
				cur->min = 2 * x + y * 16;
				cur->bits = 0;
			}
		}
		mmt.nodes.push_back(nodeL0);

		std::vector<unsigned int> chunkDims = { 2, 2 };
		std::vector<unsigned int> chunkNum = { 4, 4 };

		std::vector<int> expectedMax = {
			27, 31,
			59, 63
		};
		std::vector<int> expectedMin = {
			0, 4,
			32, 36,
		};
		//////////////////////////////

		mmt.buildIntermediateMMT(level, &chunkDims, &chunkNum);

		for (size_t i = 0; i < chunkNum[0] * chunkNum[1] / pow(2, 2); i++)
		{
			EXPECT_EQ(mmt.nodes[1][i].max, expectedMax[i]);
			EXPECT_EQ(mmt.nodes[1][i].min, expectedMin[i]);
		}
	}

	TEST(caMMT, buildMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		int data[DATA_LENGTH_2D_DUMMY];
		std::vector<unsigned int> chunkDim = { 2, 2 };
		std::vector<unsigned int> dims = { 8, 8 };

		build2DDummy(data, DATA_LENGTH_2D_DUMMY);
		caMMT<unsigned int, int> mmt;
		//////////////////////////////

		mmt.buildMMT(data, DATA_LENGTH_2D_DUMMY, &dims, &chunkDim, 3);

		EXPECT_EQ(mmt.nodes.size(), 3);	// Level 0~2
		EXPECT_EQ(mmt.nodes[2][0].min, 0);
		EXPECT_EQ(mmt.nodes[2][0].max, 63);
	}

	TEST(caMMT, msb)
	{
		//////////////////////////////
		// Char type
		char c0 = 0;
		char c4 = 4;
		char c100 = 100;
		char c101 = 101;
		char c127 = 127;

		EXPECT_EQ(msb<char>(c127), 7);
		EXPECT_EQ(msb<char>(c4), 3);
		EXPECT_EQ(msb<char>(c0), 0);
		EXPECT_EQ(msb<char>(c100, 2), 6);
		EXPECT_EQ(msb<char>(c101, 2), 6);


		//////////////////////////////
		// Integer type
		int i55858578 = 55858578;	// 11 0101 0100 0101 0101 1001 0010

		EXPECT_EQ(msb<int>(i55858578), 26);


		//////////////////////////////
		// Integer type with order option
		EXPECT_EQ(msb<int>(i55858578, 3), 23);
	}

	TEST(caMMT, serialize)
	{
		//////////////////////////////
		// Build Dummy Data
		int data[DATA_LENGTH_2D_DUMMY];
		std::vector<unsigned int> chunkDims = { 2, 2 };
		std::vector<unsigned int> dims = { 8, 8 };

		build2DDummy_s8x8(data, DATA_LENGTH_2D_DUMMY);
		caMMT<unsigned int, int> mmt;
		bstream bs;
		//////////////////////////////

		mmt.buildMMT(data, DATA_LENGTH_2D_DUMMY, &dims, &chunkDims, 3);
		mmt.serialize(bs);

		//////////////////////////////
		const char* converted = bs.c_str();
		int expected[DATA_LENGTH_2D_DUMMY] = { 0 };
		expected2DDummy_s8x8(expected, DATA_LENGTH_2D_DUMMY);
		for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
		{
			EXPECT_EQ(converted[i], expected[i]);
			std::cout << std::dec << "[" << i << "]: " << std::setfill('0') << std::setw(1) << std::hex << (short)converted[i] << std::endl;
		}
		std::cout << std::endl;
	}

	void build2DDummy(int* output, size_t length)
	{
		for (int i = 0; i < length; i++)
		{
			output[i] = i;
		}
	}

	// with sign
	void build2DDummy_s8x8(int* output, size_t length)
	{
		assert(length == 8 * 8);
		const static char arr[8][8] = {
			{105, 91, 94, 66,		6, 26, 52, 40	},
			{115, 101, 74, 50,		4, 32, -40, -56	},
			{108, 100, 75, 71,		24, 12, -1, 3	},
			{100, 96, 79, 67,		30, 18, -3, -3	},
			{115, 103, 93, 103,		62, 54, 54, 38	},
			{109, 101, 95, 113,		48, 44, 64, 44	},
			{118, 110, 105, 107,	63, 59, 63, 57	},
			{108, 100, 81, 87,		55, 47, 49, 47	}
		};

		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				output[y * 8 + x] = arr[y][x];
			}
		}
	}

	void expected2DDummy_s8x8(int* output, size_t length)
	{
		assert(length > 21);
		const static char arr[21] = {
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

}
