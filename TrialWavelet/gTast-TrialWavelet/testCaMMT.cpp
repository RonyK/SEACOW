#include "pch.h"
#include "../TrialWavelet/mmt.h"

namespace caWavelet
{
#define DATA_LENGTH_2D_DUMMY	64

	void build2DDummy(int* output, size_t length);

	TEST(caMMT, buildLeafMMT)
	{
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

		mmt.buildLeafMMT(data, DATA_LENGTH_2D_DUMMY, &chunkDim, &chunkNum);

		for (size_t i = 0; i < chunkNum[0] * chunkNum[1]; i++)
		{
			EXPECT_EQ(mmt.nodes[0][i].max, expectedMax[i]);
			EXPECT_EQ(mmt.nodes[0][i].min, expectedMin[i]);
		}
	}

	TEST(caMMT, buildIntermediateMMT)
	{

	}

	TEST(caMMT, buildMMT)
	{

	}

	void build2DDummy(int* output, size_t length)
	{
		for (int i = 0; i < length; i++)
		{
			output[i] = i;
		}
	}
}