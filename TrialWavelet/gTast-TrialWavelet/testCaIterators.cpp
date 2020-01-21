#include "pch.h"
#include "../TrialWavelet/iterators.h"


namespace caWavelet
{
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

	TEST(caIterators, caCoorIterator3D_basis_default)
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
						//EXPECT_THROW(it++, std::out_of_range);
					} else
					{
						++it;
					}
				}
			}
		}
	}

	TEST(caIterators, caCoorIterator3D_basis_1)
	{
		double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
		int b[] = { 2,3,4 };
		caWavelet::caCoorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);
		it.setBasisDim(1);

		EXPECT_EQ(it[0], 1);
		EXPECT_EQ(it[1], 11);
		EXPECT_EQ(it[2], 21);

		EXPECT_EQ(it.coor()[0], 0);
		EXPECT_EQ(it.coor()[1], 0);
		EXPECT_EQ(it.coor()[2], 0);
		EXPECT_EQ(*it, 1);

		for (int x = 0; x < b[2]; x++)
		{
			for (int z = 0; z < b[0]; z++)
			{
				for (int y = 0; y < b[1]; y++)
				{
					std::cout << z << ", " << y << ", " << x << ":" << *it << std::endl;

					EXPECT_EQ(it.coor()[0], z);
					EXPECT_EQ(it.coor()[1], y);
					EXPECT_EQ(it.coor()[2], x);
					EXPECT_EQ(*it, data[z * b[1] * b[2] + y * b[2] + x]);

					if (z == b[0] - 1 && y == b[1] - 1 && x == b[2] - 1)
					{
						//EXPECT_THROW(it++, std::out_of_range);
					} else
					{
						++it;
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

		//EXPECT_THROW(it++, std::out_of_range);
	}

	TEST(caIterators, caWTIteratorCalcBandDim)
	{
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		caWavelet::caWTIterator<int, double> it(data, sizeof(d) / sizeof(int), d);
		it.setMaxLevel(1);

		{
			// Level 1, Band 0
			it.setCurLevel(1);
			it.setCurBand(0);

			size_t sP_l1b0[2] = { 0, 0 };
			size_t eP_l1b0[2] = { 2, 2 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l1b0[d]);
				EXPECT_EQ(it.beP_[d], eP_l1b0[d]);
			}
		}

		{
			// Level 1, Band 1
			it.setCurLevel(1);
			it.setCurBand(1);

			size_t sP_l1b1[2] = { 0, 2 };
			size_t eP_l1b1[2] = { 2, 4 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l1b1[d]);
				EXPECT_EQ(it.beP_[d], eP_l1b1[d]);
			}
		}

		{
			// Level 1, Band 2
			it.setCurLevel(1);
			it.setCurBand(2);

			size_t sP_l1b2[2] = { 2, 0 };
			size_t eP_l1b2[2] = { 4, 2 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l1b2[d]);
				EXPECT_EQ(it.beP_[d], eP_l1b2[d]);
			}
		}

		{
			// Level 1, Band 3
			it.setCurLevel(1);
			it.setCurBand(3);

			size_t sP_l1b3[2] = { 2, 2 };
			size_t eP_l1b3[2] = { 4, 4 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l1b3[d]);
				EXPECT_EQ(it.beP_[d], eP_l1b3[d]);
			}
		}

		{
			// Level 0, Band 1
			it.setCurLevel(0);
			it.setCurBand(1);

			size_t sP_l0b1[2] = { 0, 4 };
			size_t eP_l0b1[2] = { 4, 8 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l0b1[d]);
				EXPECT_EQ(it.beP_[d], eP_l0b1[d]);
			}
		}

		{
			// Level 0, Band 2
			it.setCurLevel(0);
			it.setCurBand(2);

			size_t sP_l0b2[2] = { 4, 0 };
			size_t eP_l0b2[2] = { 8, 4 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l0b2[d]);
				EXPECT_EQ(it.beP_[d], eP_l0b2[d]);
			}
		}

		{
			// Level 0, Band 3
			it.setCurLevel(0);
			it.setCurBand(3);

			size_t sP_l0b3[2] = { 4, 4 };
			size_t eP_l0b3[2] = { 8, 8 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(it.bsP_[d], sP_l0b3[d]);
				EXPECT_EQ(it.beP_[d], eP_l0b3[d]);
			}
		}
	}

	TEST(caIterators, caWTIteratorMoveTo)
	{
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		caWavelet::caWTIterator<int, double> it(data, sizeof(d) / sizeof(int), d);
		it.setMaxLevel(1);
		it.setCurLevel(1);
		it.setCurBand(0);

		int c[2] = { 3, 3 };
		it.moveTo(caWavelet::caCoor<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[15]);

		c[0] = 6;
		c[1] = 2;
		it.moveTo(caWavelet::caCoor<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[42]);

		c[0] = 0;
		c[1] = 1;
		it.moveTo(caWavelet::caCoor<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[1]);

		c[0] = 6;
		c[1] = 6;
		it.moveTo(caWavelet::caCoor<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[58]);
	}

	TEST(caIterators, caWTIterator2D)
	{
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		caWavelet::caWTIterator<int, double> it(data, sizeof(d) / sizeof(int), d);
		it.setMaxLevel(1);
		it.setCurLevel(1);
		it.setCurBand(0);

		// Simple iterating test
		//EXPECT_EQ(it[0], 0);
		//EXPECT_EQ(it[1], 1);
		//EXPECT_EQ(it[2], 2);
		//EXPECT_THROW(it[16], std::out_of_range);

		// Level 1
		int sizeLevel_1 = pow(2, 2);
		for (int b = 0; b < sizeLevel_1; b++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int x = 0; x < 2; x++)
				{
					std::cout << "b: " << b << ", y: " << y << "(" << (y + (b & 0x02)) << "), x: " << x << "(" << (x + (b & 0x01) * 2) << ")" << std::endl;
					std::cout << "coor[0]: " << it.coor()[0] << ", coor[1]: " << it.coor()[1] << std::endl;
					std::cout << b * sizeLevel_1 + y * 2 + x << std::endl;
					EXPECT_EQ(it.coor()[0], y + (b & 0x02));
					EXPECT_EQ(it.coor()[1], x + (b & 0x01) * 2);
					EXPECT_EQ(*it, b * sizeLevel_1 + y * 2 + x);
					++it;
				}
			}
		}
	}
}
