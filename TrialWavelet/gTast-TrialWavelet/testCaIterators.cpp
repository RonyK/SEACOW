#include "pch.h"
#include "../TrialWavelet/iterators.h"

TEST(caIterators, caCoor)
{
	// Initalized into 0
	caWavelet::caCoor<int> coorDefault(3);
	EXPECT_EQ(coorDefault.size(), 3);

	EXPECT_EQ(coorDefault[0], 0);
	EXPECT_EQ(coorDefault[1], 0);
	EXPECT_EQ(coorDefault[2], 0);

	int c[3] = { 1, 2, 3 };
	caWavelet::caCoor<int> coorInit(3, c);
	EXPECT_EQ(coorInit[0], 1);
	EXPECT_EQ(coorInit[1], 2);
	EXPECT_EQ(coorInit[2], 3);

	EXPECT_EQ(coorInit.at(0), 1);
	EXPECT_EQ(coorInit.at(1), 2);
	EXPECT_EQ(coorInit.at(2), 3);

	int* d = coorInit.data();
	EXPECT_EQ(*(d++), 1);
	EXPECT_EQ(*(d++), 2);
	EXPECT_EQ(*(d), 3);
}

TEST(caIterators, caCoorIterator1D)
{
	double data[] = { 1,2,3,4,5,6,7,8 };
	int b[] = { 8 };
	caWavelet::caCoorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);

	EXPECT_EQ(it[0], 1);
	EXPECT_EQ(it[1], 2);
	EXPECT_EQ(it[2], 3);
	EXPECT_THROW(it[8], std::out_of_range);

	EXPECT_EQ(it.coor()[0], 0);
	EXPECT_EQ(*it++, 1);

	EXPECT_EQ(it.coor()[0], 1);
	EXPECT_EQ(*it++, 2);

	EXPECT_EQ(it.coor()[0], 2);
	EXPECT_EQ(*it++, 3);

	EXPECT_EQ(it.coor()[0], 3);
	EXPECT_EQ(*it++, 4);
}

TEST(caIterators, caCoorIterator3D)
{
	double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
	int b[] = { 2,3,4 };
	caWavelet::caCoorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);

	EXPECT_EQ(it[0], 1);
	EXPECT_EQ(it[1], 2);
	EXPECT_EQ(it[2], 3);

	EXPECT_EQ(it.coor()[0], 0);
	EXPECT_EQ(it.coor()[1], 0);
	EXPECT_EQ(it.coor()[2], 0);
	EXPECT_EQ(*it, 1);

	for (int z = 0; z < b[0]; z++)
	{
		for (int y = 0; y < b[1]; y++)
		{
			for (int x = 0; x < b[2]; x++)
			{
				std::cout << z << ", " << y << ", " << x << ":" << *it << std::endl;

				EXPECT_EQ(it.coor()[0], z);
				EXPECT_EQ(it.coor()[1], y);
				EXPECT_EQ(it.coor()[2], x);
				EXPECT_EQ(*it, data[z*b[1]*b[2] + y*b[2] + x]);

				if (z == b[0] - 1 && y == b[1] - 1 && x == b[2] - 1)
				{
					EXPECT_THROW(it++, std::out_of_range);
				} else
				{
					it++;
				}
			}
		}
	}
}

TEST(caIterators, caCoorRangeIterator1D)
{
	double data[] = { 1,2,3,4,5,6,7,8 };
	int b[] = { 8 };
	int s[] = { 2 };
	int e[] = { 5 };
	caWavelet::caCoorRangeIterator<int, double> it(data, sizeof(b) / sizeof(int), b, s, e);

	EXPECT_EQ(it[0], 3);
	EXPECT_EQ(it[1], 4);
	EXPECT_EQ(it[2], 5);

	caWavelet::caCoor<int> cc = it.coor();

	EXPECT_EQ(it.coor()[0], 2);
	EXPECT_EQ(*it++, 3);

	EXPECT_EQ(it.coor()[0], 3);
	EXPECT_EQ(*it++, 4);

	EXPECT_EQ(it.coor()[0], 4);
	EXPECT_EQ(*it, 5);

	EXPECT_THROW(it++, std::out_of_range);
}
