#include "pch.h"
#include "dummy.h"
#include "../TrialWavelet/mmt.h"

namespace caWavelet
{
#define DATA_LENGTH_2D_DUMMY	64

	void build2DDummy(int* output, size_t length);

	TEST(caMMT, buildLeafMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		int data[DATA_LENGTH_2D_DUMMY];
		std::vector<unsigned int> dim = { 8, 8 };
		std::vector<unsigned int> chunkDim = { 2, 2 };
		std::vector<unsigned int> chunkNum = { 4, 4 };
		size_t maxLevel = 0;

		build2DDummy(data, DATA_LENGTH_2D_DUMMY);
		caMMT<unsigned int, int> mmt(dim, chunkDim, maxLevel);
		
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

		mmt.forwardBuildLeaf(data, DATA_LENGTH_2D_DUMMY);

		for (size_t i = 0; i < (size_t)chunkNum[0] * chunkNum[1]; i++)
		{
			EXPECT_EQ(mmt.nodes_[0][i].max, expectedMax[i]);
			EXPECT_EQ(mmt.nodes_[0][i].min, expectedMin[i]);
		}
	}

	TEST(caMMT, buildIntermediateMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		std::vector<unsigned int> dim = { 8, 8 };
		std::vector<unsigned int> chunkDim = { 2, 2 };
		std::vector<unsigned int> chunkNum = { 4, 4 };
		size_t maxLevel = 1;
		caMMT<unsigned int, int> mmt(dim, chunkDim, maxLevel);
		

		std::vector<caMMT<unsigned int, int>::mmtNode> nodeL0(16);
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				size_t i = x + 4 * y;
				nodeL0[i].max = 2 * x + y * 16 + 9;
				nodeL0[i].min = 2 * x + y * 16;
				nodeL0[i].bits = 0;
			}
		}
		mmt.nodes_.push_back(nodeL0);

		std::vector<int> expectedMax = {
			27, 31,
			59, 63
		};
		std::vector<int> expectedMin = {
			0, 4,
			32, 36,
		};
		//////////////////////////////

		mmt.forwardBuildNonLeaf(maxLevel);

		for (size_t i = 0; i < (size_t)chunkNum[0] * chunkNum[1] / pow(2, 2); i++)
		{
			EXPECT_EQ(mmt.nodes_[1][i].max, expectedMax[i]);
			EXPECT_EQ(mmt.nodes_[1][i].min, expectedMin[i]);
		}
	}

	TEST(caMMT, buildMMT)
	{
		//////////////////////////////
		// Build Dummy Data
		int data[DATA_LENGTH_2D_DUMMY];
		std::vector<unsigned int> dim = { 8, 8 };
		std::vector<unsigned int> chunkDim = { 2, 2 };
		size_t maxLevel = 2;

		build2DDummy(data, DATA_LENGTH_2D_DUMMY);
		caMMT<unsigned int, int> mmt(dim, chunkDim, maxLevel);
		//////////////////////////////

		mmt.buildMMT(data, DATA_LENGTH_2D_DUMMY);

		EXPECT_EQ(mmt.nodes_.size(), maxLevel + 1);	// Level 0~2 (maxLevel)
		EXPECT_EQ(mmt.nodes_[2][0].min, 0);
		EXPECT_EQ(mmt.nodes_[2][0].max, 63);
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

	namespace caDummy
	{
		namespace data2D_sc8x8
		{
			TEST(caMMT, mmt_serialize_sc8x8)
			{
				//////////////////////////////
				// Build Dummy Data
				value_type data[DATA_LENGTH_2D_DUMMY];
				std::vector<unsigned char> dim = { 8, 8 };
				std::vector<unsigned char> chunkDim = { 2, 2 };
				size_t maxLevel = 2;

				getDummy(data, DATA_LENGTH_2D_DUMMY);
				caMMT<unsigned char, char> mmt(dim, chunkDim, maxLevel);
				bstream bs;
				//////////////////////////////

				mmt.buildMMT(data, DATA_LENGTH_2D_DUMMY);
				mmt.serialize(bs);

				//////////////////////////////
				const unsigned char* converted = bs.data();
				char expected[DATA_LENGTH_2D_DUMMY] = { 0 };
				getExMMT(expected, DATA_LENGTH_2D_DUMMY);
				for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
				{
					EXPECT_EQ(converted[i], (unsigned char)expected[i]);
					std::cout << std::dec << "[" << i << "]: " << std::setfill('0') << std::setw(1) << std::hex << (short)converted[i] << std::endl;
				}
				std::cout << std::endl;
			}
		}
	}

	void build2DDummy(int* output, size_t length)
	{
		for (int i = 0; i < length; i++)
		{
			output[i] = i;
		}
	}
}
