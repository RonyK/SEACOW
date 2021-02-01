#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
	TEST(coordinate, coordinateBasic)
	{
		// Initalized into 0
		msdb::coordinate<int> coorDefault(3);
		EXPECT_EQ(coorDefault.size(), 3);

		EXPECT_EQ(coorDefault[0], 0);
		EXPECT_EQ(coorDefault[1], 0);
		EXPECT_EQ(coorDefault[2], 0);

		int c[3] = { 1, 2, 3 };
		msdb::coordinate<int> coorInit(3, c);
		EXPECT_EQ(coorInit[0], 1);
		EXPECT_EQ(coorInit[1], 2);
		EXPECT_EQ(coorInit[2], 3);

		EXPECT_EQ(coorInit.at(0), 1);
		EXPECT_EQ(coorInit.at(1), 2);
		EXPECT_EQ(coorInit.at(2), 3);

		//int* d = coorInit.data();
		//EXPECT_EQ(*(d++), 1);
		//EXPECT_EQ(*(d++), 2);
		//EXPECT_EQ(*(d), 3);
	}

	TEST(coorIterator, coorIterator1D)
	{
		//double data[] = { 1,2,3,4,5,6,7,8 };
		//int b[] = { 8 };
		//msdb::coorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 2);
		//EXPECT_EQ(it[2], 3);
		////EXPECT_THROW(it[8], msdb_exception());

		//EXPECT_EQ(it.coor()[0], 0);
		//EXPECT_EQ(*it++, 1);

		//EXPECT_EQ(it.coor()[0], 1);
		//EXPECT_EQ(*it++, 2);

		//EXPECT_EQ(it.coor()[0], 2);
		//EXPECT_EQ(*it++, 3);

		//EXPECT_EQ(it.coor()[0], 3);
		//EXPECT_EQ(*it++, 4);
	}

	TEST(coorIterator, coorIterator3D_basis_default)
	{
		//double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
		//int b[] = { 2,3,4 };
		//msdb::coorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 2);
		//EXPECT_EQ(it[2], 3);

		//EXPECT_EQ(it.coor()[0], 0);
		//EXPECT_EQ(it.coor()[1], 0);
		//EXPECT_EQ(it.coor()[2], 0);
		//EXPECT_EQ(*it, 1);

		//for (int z = 0; z < b[0]; z++)
		//{
		//	for (int y = 0; y < b[1]; y++)
		//	{
		//		for (int x = 0; x < b[2]; x++)
		//		{
		//			std::cout << z << ", " << y << ", " << x << ":" << *it << std::endl;

		//			EXPECT_EQ(it.coor()[0], z);
		//			EXPECT_EQ(it.coor()[1], y);
		//			EXPECT_EQ(it.coor()[2], x);
		//			EXPECT_EQ(*it, data[z * b[1] * b[2] + y * b[2] + x]);

		//			if (z == b[0] - 1 && y == b[1] - 1 && x == b[2] - 1)
		//			{
		//				//EXPECT_THROW(it++, std::out_of_range);
		//			} else
		//			{
		//				++it;
		//			}
		//		}
		//	}
		//}
	}

	TEST(coorIterator, coorIterator3D_basis_1)
	{
		//double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
		//int b[] = { 2,3,4 };
		//msdb::coorIterator<int, double> it(data, sizeof(b) / sizeof(int), b);
		//it.setBasisDim(1);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 11);
		//EXPECT_EQ(it[2], 21);

		//EXPECT_EQ(it.coor()[0], 0);
		//EXPECT_EQ(it.coor()[1], 0);
		//EXPECT_EQ(it.coor()[2], 0);
		//EXPECT_EQ(*it, 1);

		//for (int x = 0; x < b[2]; x++)
		//{
		//	for (int z = 0; z < b[0]; z++)
		//	{
		//		for (int y = 0; y < b[1]; y++)
		//		{
		//			std::cout << z << ", " << y << ", " << x << ":" << *it << std::endl;

		//			EXPECT_EQ(it.coor()[0], z);
		//			EXPECT_EQ(it.coor()[1], y);
		//			EXPECT_EQ(it.coor()[2], x);
		//			EXPECT_EQ(*it, data[z * b[1] * b[2] + y * b[2] + x]);

		//			if (z == b[0] - 1 && y == b[1] - 1 && x == b[2] - 1)
		//			{
		//				//EXPECT_THROW(it++, std::out_of_range);
		//			} else
		//			{
		//				++it;
		//			}
		//		}
		//	}
		//}
	}

	TEST(coorRangeIterator, coorRangeIterator1D)
	{
		//double data[] = { 1,2,3,4,5,6,7,8 };
		//int b[] = { 8 };
		//int s[] = { 2 };
		//int e[] = { 5 };
		//msdb::coorRangeIterator<int, double> it(data, sizeof(b) / sizeof(int), b, s, e);

		//EXPECT_EQ(it[0], 3);
		//EXPECT_EQ(it[1], 4);
		//EXPECT_EQ(it[2], 5);

		//msdb::coordinate<int> cc = it.coor();

		//EXPECT_EQ(it.coor()[0], 2);
		//EXPECT_EQ(*it++, 3);

		//EXPECT_EQ(it.coor()[0], 3);
		//EXPECT_EQ(*it++, 4);

		//EXPECT_EQ(it.coor()[0], 4);
		//EXPECT_EQ(*it, 5);

		////EXPECT_THROW(it++, std::out_of_range);
	}

	//////////////////////////////
	// coordinateIterator
	TEST(itemIterator, coordinateIterator1D)
	{
		double data[] = { 1,2,3,4,5,6,7,8 };
		int b[] = { 8 };
		itemIterator<int> it((void*)data, eleType::DOUBLE, sizeof(b) / sizeof(int), b);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 2);
		//EXPECT_EQ(it[2], 3);
		//EXPECT_THROW(it[8], std::out_of_range);

		EXPECT_EQ(it.coor()[0], 0);
		EXPECT_EQ((*it).getDouble(), data[0]);
		++it;

		EXPECT_EQ(it.coor()[0], 1);
		EXPECT_EQ((*it).getDouble(), data[1]);
		++it;

		EXPECT_EQ(it.coor()[0], 2);
		EXPECT_EQ((*it).getDouble(), data[2]);
		++it;

		EXPECT_EQ(it.coor()[0], 3);
		EXPECT_EQ((*it).getDouble(), data[3]);
		++it;
	}

	TEST(coordinateIterator, coorIterator3D_basis_default)
	{
		double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
		int b[] = { 2,3,4 };
		msdb::itemIterator<int> it((void*)data, eleType::DOUBLE, sizeof(b) / sizeof(int), b);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 2);
		//EXPECT_EQ(it[2], 3);

		EXPECT_EQ(it.coor()[0], 0);
		EXPECT_EQ(it.coor()[1], 0);
		EXPECT_EQ(it.coor()[2], 0);
		EXPECT_EQ((*it).getDouble(), data[0]);

		for (int z = 0; z < b[0]; z++)
		{
			for (int y = 0; y < b[1]; y++)
			{
				for (int x = 0; x < b[2]; x++)
				{
					std::cout << z << ", " << y << ", " << x << ":" << (*it).getDouble() << std::endl;

					EXPECT_EQ(it.coor()[0], z);
					EXPECT_EQ(it.coor()[1], y);
					EXPECT_EQ(it.coor()[2], x);
					EXPECT_EQ((*it).getDouble(), data[z * b[1] * b[2] + y * b[2] + x]);

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

	TEST(coordinateIterator, coorIterator3D_basis_1)
	{
		double data[] = { 1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44, 51,52,53,54 };
		int b[] = { 2,3,4 };
		double output;
		msdb::itemIterator<int> it((void*)data, eleType::DOUBLE, sizeof(b) / sizeof(int), b);
		it.setBasisDim(1);

		//EXPECT_EQ(it[0], 1);
		//EXPECT_EQ(it[1], 11);
		//EXPECT_EQ(it[2], 21);

		EXPECT_EQ(it.coor()[0], 0);
		EXPECT_EQ(it.coor()[1], 0);
		EXPECT_EQ(it.coor()[2], 0);
		EXPECT_EQ((*it).getDouble(), 1);
		(*it).getData(&output);
		EXPECT_EQ(output, 1);

		for (int x = 0; x < b[2]; x++)
		{
			for (int z = 0; z < b[0]; z++)
			{
				for (int y = 0; y < b[1]; y++)
				{
					std::cout << z << ", " << y << ", " << x << ":" << (*it).getDouble() << std::endl;

					EXPECT_EQ(it.coor()[0], z);
					EXPECT_EQ(it.coor()[1], y);
					EXPECT_EQ(it.coor()[2], x);
					EXPECT_EQ((*it).getDouble(), data[z * b[1] * b[2] + y * b[2] + x]);
					(*it).getData(&output);
					EXPECT_EQ(output, data[z * b[1] * b[2] + y * b[2] + x]);

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

	TEST(itemRangeIterator, itemRangeIterator1D)
	{
		double data[] = { 1,2,3,4,5,6,7,8 };
		int b[] = { 8 };
		int s[] = { 2 };
		int e[] = { 5 };
		itemRangeIterator<int> it((void*)data, eleType::DOUBLE, sizeof(b) / sizeof(int), b, s, e);
		coordinate<int> cc = it.coor();

		EXPECT_EQ(it.coor()[0], 2);
		EXPECT_EQ((*it).getDouble(), data[2]);
		++it;

		EXPECT_EQ(it.coor()[0], 3);
		EXPECT_EQ((*it).getDouble(), data[3]);
		++it;

		EXPECT_EQ(it.coor()[0], 4);
		EXPECT_EQ((*it).getDouble(), data[4]);
		++it;
	}

}
