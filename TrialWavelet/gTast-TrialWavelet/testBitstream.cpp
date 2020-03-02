#include "pch.h"
#include "../TrialWavelet/mmt.h"
#include "../TrialWavelet/bitstream.h"
#include <bitset>

namespace caWavelet
{

	TEST(caBitstream, bitstream)
	{
		bitstream<1> bs;

		std::bitset<1> b1;
		std::bitset<2> b2;
		std::bitset<3> b3;
		std::bitset<8> b8;

		b1.set(0);
		b2.set(0);
		b3.set(0);
		b8 = 0x36;

		bs << b8;
		//bs << b1;
		//bs << b1;
		//bs << b2 << b3;

		char c[10] = {0};
		bs.toChar(c, 100);

		for (int i = 0; i < (bs.size() + 7) / 8; i++)
		{
			std::bitset<8> bt8 = c[i];
			std::cout << bt8 << " ";
		}

		std::cout << std::endl;
		std::cout << "test" << std::endl;
	}
}