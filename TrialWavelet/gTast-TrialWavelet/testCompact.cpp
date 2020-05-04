#include "pch.h"
#include "dummy.h"
#include "../TrialWavelet/compact.h"

namespace caWavelet
{
	namespace caDummy
	{
		namespace data2D_s4x4
		{
			TEST(caCompact, d2_s4x4_compact)
			{
				//////////////////////////////
				// Build Dummy Data
				char data[dummyDataLength];
				char wtData[dummyDataLength];
				char expected[dummyDataLength];

				getDummy(data, dummyDataLength);
				getWTDummy(wtData, dummyDataLength);
				getEXDummy(expected, dummyDataLength);
				//////////////////////////////

				caMMT<dim_type, value_type> mmt(dims, chunkDims, maxLevel);
				mmt.buildMMT(data, dummyDataLength);

				bstream bs;
				caCompact<dim_type, value_type> compact(dims, chunkNums, maxLevel);

				compact.encode(bs, wtData, mmt);

				const char* converted = bs.c_str();
				for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
				{
					std::cout << std::dec << "[" << i << "]" << std::setfill('0') << std::setw(2) << std::right << std::hex << (short)converted[i] << std::endl;
					EXPECT_EQ((unsigned char)expected[i], (unsigned char)converted[i]);
				}
			}
		}
	}
}