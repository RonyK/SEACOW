#include <pch.h>
#include <util/coordinate.h>
#include <util/waveletIterator.h>

namespace msdb
{
	TEST(waveletIterator, waveletIteratorCalcBandDim)
	{
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		msdb::waveletIterator<int, double> it(data, sizeof(d) / sizeof(int), d);
		it.setMaxLevel(1);

		auto bsP = it.getBandStartPoint();
		auto beP = it.getBandEndPoint();

		{
			// Level 1, Band 0
			it.setCurLevel(1);
			it.setCurBand(0);

			size_t sP_l1b0[2] = { 0, 0 };
			size_t eP_l1b0[2] = { 2, 2 };

			for (int d = 0; d < 2; d++)
			{
				EXPECT_EQ(bsP[d], sP_l1b0[d]);
				EXPECT_EQ(beP[d], eP_l1b0[d]);
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
				EXPECT_EQ(bsP[d], sP_l1b1[d]);
				EXPECT_EQ(beP[d], eP_l1b1[d]);
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
				EXPECT_EQ(bsP[d], sP_l1b2[d]);
				EXPECT_EQ(beP[d], eP_l1b2[d]);
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
				EXPECT_EQ(bsP[d], sP_l1b3[d]);
				EXPECT_EQ(beP[d], eP_l1b3[d]);
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
				EXPECT_EQ(bsP[d], sP_l0b1[d]);
				EXPECT_EQ(beP[d], eP_l0b1[d]);
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
				EXPECT_EQ(bsP[d], sP_l0b2[d]);
				EXPECT_EQ(beP[d], eP_l0b2[d]);
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
				EXPECT_EQ(bsP[d], sP_l0b3[d]);
				EXPECT_EQ(beP[d], eP_l0b3[d]);
			}
		}
	}

	TEST(waveletIterator, waveletIteratorMoveTo)
	{
		size_t maxLevel = 1;
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		waveletIterator<int, double> it(data, sizeof(d) / sizeof(int), d);
		it.setMaxLevel(maxLevel);
		it.setCurLevel(1);
		it.setCurBand(0);

		int c[2] = { 3, 3 };

		it.moveTo(coordinate<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[15]);

		c[0] = 6;
		c[1] = 2;
		it.moveTo(coordinate<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[42]);

		c[0] = 0;
		c[1] = 1;
		it.moveTo(coordinate<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[1]);

		c[0] = 6;
		c[1] = 6;
		it.moveTo(coordinate<int>(2, c));
		EXPECT_EQ(it.coor()[0], c[0]);
		EXPECT_EQ(it.coor()[1], c[1]);
		EXPECT_EQ(*it, data[58]);
		EXPECT_EQ(it.curLevel(), 0);
		EXPECT_EQ(it.curBand(), 3);

		it.moveToStart();
		EXPECT_EQ(it.coor()[0], 0);
		EXPECT_EQ(it.coor()[1], 0);
		EXPECT_EQ(*it, data[0]);
		EXPECT_EQ(it.curLevel(), maxLevel);
		EXPECT_EQ(it.curBand(), 0);
	}

	TEST(waveletIterator, waveletIterator2D)
	{
		double data[64] = { 0 };	// WT data
		int d[] = { 8, 8 };			// Dimension

		// Data initialization
		for (int i = 0; i < 64; i++)
		{
			data[i] = i;
		}

		// Create WTIterator
		waveletIterator<int, double> it(data, sizeof(d) / sizeof(int), d, 1);
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
					std::cout << "b: " << b << ", y: " << y << "(" << (y + (b & 0x02)) << ", " << it.coor()[0] << "), x: " << x << "(" << (x + (b & 0x01) * 2) << ", " << it.coor()[1] << ")" << std::endl;
					std::cout << b * sizeLevel_1 + y * 2 + x << ", " << *it << std::endl;
					EXPECT_EQ(it.coor()[0], y + (b & 0x02));
					EXPECT_EQ(it.coor()[1], x + (b & 0x01) * 2);
					EXPECT_EQ(*it, b * sizeLevel_1 + y * 2 + x);
					++it;
				}
			}
		}
	}

	TEST(waveletRangeIterator, waveletRangeIterator2D)
	{
		double data[256] = { 0 };	// WT data
		int d[] = { 16, 16 };		// Dimension
		size_t level = 1;
		int bandSize_L1[] = { d[0] / std::pow(2, level + 1), d[1] / std::pow(2, level + 1) };

		// Data initialization
		for (int i = 0; i < 256; i++)
		{
			data[i] = i;
		}

		// Test 01 in level 1, band 0
		{
			int s[] = { 1, 2 };
			int e[] = { 3, 4 };
			// Create WTIterator
			waveletRangeIterator<int, double> it(data, sizeof(d) / sizeof(int), d, s, e, level);

			// Level 1
			int length = (s[0] - e[0]) * (s[1] - e[1]);

			for (int y = s[0]; y < e[0]; y++)
			{
				for (int x = s[1]; x < e[1]; x++)
				{
					std::cout << "y: " << y << "(" << y << "), x: " << x << "(" << x << ")" << std::endl;
					std::cout << "coor[0]: " << it.coor()[0] << ", coor[1]: " << it.coor()[1] << std::endl;
					std::cout << y * bandSize_L1[1] + x << " / " << *it << std::endl;
					EXPECT_EQ(it.coor()[0], y);
					EXPECT_EQ(it.coor()[1], x);
					EXPECT_EQ(*it, y * bandSize_L1[1] + x);
					++it;
				}
			}
		}
	}
}
