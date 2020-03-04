#include "pch.h"
#include "../TrialWavelet/bitstringstream.h"
#include <bitset>

namespace caWavelet
{
	TEST(caBitStringStream, bstream_bistset)
	{
		unsigned char expected[2] = {
			0x36, // 0011 0110
			0xD2  // 1101 0010
		};

		bstream bs;

		std::bitset<1> b1;
		std::bitset<2> b2;
		std::bitset<3> b3;
		std::bitset<8> b8;

		b1.set(0);	// 1
		b2.set(0);	// 01
		b3.set(0);	// 001
		b8 = 0x36;	// 0011 0110

		bs << b8 << b1 << b1 << b2 << b3;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.c_str();
		for (int i = 0; i < (bs.size() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<unsigned char>(str[i]), expected[i]);
		}

		// Copy test
		unsigned char c[10] = { 0 };
		bs.toChar(c, 100);
		for (int i = 0; i < (bs.size() + 7) / 8; i++)
		{
			EXPECT_EQ(c[i], expected[i]);
		}
	}

	TEST(caBitStringStream, u32bstream_bitset)
	{
		unsigned char cExpected[6] = {
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

		u32bstream bs;

		std::bitset<3> b3;
		std::bitset<8> b8;
		std::bitset<16> b16;
		std::bitset<21> b21;

		b3 = 0x1;		// 001
		b8 = 0x36;		// 0011 0110
		b16 = 0x8421;	// 1000 0100 0010 0001
		b21 = 0x1E1861;	// 1111 0000 1100 0011 0000 1

		bs << b3 << b8 << b16 << b21;


		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char32_t* str = bs.c_str();
		for (int i = 0; i < (bs.size() + 31) / 32; i++)
		{
			EXPECT_EQ(static_cast<char32_t>(str[i]), u32Expected[i]);
		}

		// Copy test
		unsigned char c[10] = { 0 };
		bs.toChar(c, 100);
		for (int i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
		{
			EXPECT_EQ(c[i], cExpected[i]);
		}
	}

	TEST(caBitStringStream, bstream_integer)
	{
		bstream bs;

		unsigned int i3 = 3;		// (~) 0000 0011
		unsigned int i175 = 175;	// (~) 1010 1111

		unsigned char expected[8] = {
			0x00, 0x00, 0x00, 0x03, // 0000 0011
			0x00, 0x00, 0x00, 0xAF  // 1010 1111
		};

		bs << i3 << i175;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.c_str();
		for (int i = 0; i < (bs.size() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<unsigned char>(str[i]), expected[i]);
		}
	}

	TEST(caBitStringStream, bstream_bitset_integer)
	{
		bstream bs;

		std::bitset<3> b3 = 0x1;;	// 001
		unsigned int i3 = 3;		// (~) 0000 0011
		unsigned int i175 = 175;	// (~) 1010 1111

		unsigned char expected[9] = {
			0x20, 0x00, 0x00, 0x00, // 0010 0000
			0x60, 0x00, 0x00, 0x15, // 0110 0000 (~) 0001 0101
			0xE0 // 111_
		};
		
		bs << b3 << i3 << i175;

		//////////////////////////////
		// Output test				//
		//////////////////////////////
		const char* str = bs.c_str();
		for (int i = 0; i < (bs.size() + 7) / 8; i++)
		{
			EXPECT_EQ(static_cast<unsigned char>(str[i]), expected[i]);
		}
	}

}