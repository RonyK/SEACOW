#include <pch_test.h>
#include <io/bitstream.h>
#include <bitset>

namespace msdb
{
namespace core
{
	TEST(bitStringStream, bstream_bistset)
	{
		bstream bs;

		std::bitset<1> b1 = 0x1;		// 1
		std::bitset<2> b2 = 0x1;		// 01
		std::bitset<3> b3 = 0x1;		// 001
		std::bitset<8> b8 = 0x36;		// 0011 0110

		char expected[2] = {
			0x36, // 0011 0110
			0xD2  // 1101 0010
		};

		bs << b8 << b1 << b1 << b2 << b3;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.data();
		int s = bs.sizebits();
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<char>(str[i]), expected[i]);
		}
	}

	TEST(caBitStringStream, u32bstream_bitset)
	{
		u32bstream bs;

		std::bitset<3> b3 = 0x1;		// 001
		std::bitset<8> b8 = 0x36;		// 0011 0110
		std::bitset<16> b16 = 0x8421;	// 1000 0100 0010 0001;
		std::bitset<21> b21 = 0x1E1861;	// 1111 0000 1100 0011 0000 1

		char cExpected[6] = {
			0x26,	// 0010 0110 
			0xD0,	// 1101 0000
			0x84,	// 1000 0100
			0x3E,	// 0011 1110 
			0x18,	// 0001 1000 
			0x61	// 0110 0001
		};

		char32_t u32Expected[2] = {
			0x26D0843E,	// 0010 0110 // 1101 0000 // 1000 0100 // 0011 1110 
			0x18610000,	// 0001 1000 // 0110 0001
		};

		bs << b3 << b8 << b16 << b21;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char32_t* str = bs.data();
		for (int i = 0; i < (bs.sizebits() + 31) / 32; i++)
		{
			EXPECT_EQ(static_cast<char32_t>(str[i]), u32Expected[i]);
		}
	}

	TEST(caBitStringStream, bstream_integer)
	{
		bstream bs;

		unsigned int i3 = 3;		// (~) 0000 0011
		unsigned int i175 = 175;	// (~) 1010 1111

		char expected[8] = {
			0x00, 0x00, 0x00, 0x03, // 0000 0011
			0x00, 0x00, 0x00, 0xAF  // 1010 1111
		};

		bs << i3 << i175;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.data();
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<char>(str[i]), expected[i]);
		}
	}

	TEST(caBitStringStream, bstream_bitset_integer)
	{
		bstream bs;

		std::bitset<3> b3 = 0x1;;	// 001
		unsigned int i3 = 3;		// (~) 0000 0011
		unsigned int i175 = 175;	// (~) 1010 1111

		char expected[9] = {
			0x20, 0x00, 0x00, 0x00, // 0010 0000
			0x60, 0x00, 0x00, 0x15, // 0110 0000 (~) 0001 0101
			0xE0 // 111_
		};

		bs << b3 << i3 << i175;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.data();
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<char>(str[i]), expected[i]);
		}
	}

	TEST(caBitStringStream, bstream_manip)
	{
		bstream bs;

		std::bitset<1> b1 = 0x01;		//		   1
		std::bitset<5> b5 = 0x01;		//    0 0001
		std::bitset<6> b6 = 0x01;		//   00 0001
		std::bitset<7> b7 = 0x01;		//  000 0001
		std::bitset<8> b8 = 0x36;		// 0011 0110

		char expected[3] = {
			0x08,	// 0000 1000
			0x43,	// 0100 0011
			0x36	// 0011 0110
		};

		bs << b5 << setw(5) << b6 << b7 << b1 << setw(0) << b8;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.data();
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<char>(str[i]), expected[i]);
		}
	}

	TEST(caBitStringStream, iobstream)
	{
		bstream bs;

		std::bitset<1> b1 = 0x1;		// 1
		std::bitset<2> b2 = 0x1;		// 01
		std::bitset<3> b3 = 0x1;		// 001
		std::bitset<8> b8 = 0x36;		// 0011 0110

		char expected[2] = {
			0x36, // 0011 0110
			0xD2  // 1101 0010
		};

		bs << b8 << b1 << b1 << b2 << b3;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		std::cout << "output" << std::endl;
		const char* str = bs.data();
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{

			EXPECT_EQ(static_cast<char>(str[i]), expected[i]);
		}

		char result[2];
		bs >> result[0] >> result[1];

		std::cout << "input" << std::endl;
		for (int i = 0; i < (bs.sizebits() + 7) / 8; i++)
		{
			EXPECT_EQ(result[i], expected[i]);
		}
	}
}		// core
}		// msdb