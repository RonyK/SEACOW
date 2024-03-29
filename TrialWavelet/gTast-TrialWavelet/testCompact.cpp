#include "pch.h"
#include "dummy.h"
#include "../TrialWavelet/compact.h"

namespace caWavelet
{
	namespace caDummy
	{
		namespace data2D_sc4x4
		{
			TEST(caCompact, d2_s4x4_compact)
			{
				//////////////////////////////
				// Build Dummy Data
				value_type data[dummyDataLength];
				value_type wtData[dummyDataLength];
				value_type expected[dummyDataLength];

				getDummy(data, dummyDataLength);
				getWTDummy(wtData, dummyDataLength);
				getExDummy(expected, dummyDataLength);
				//////////////////////////////

				caMMT<dim_type, value_type> mmt(dims, chunkDims, maxLevel);
				mmt.buildMMT(data, dummyDataLength);

				bstream bs;
				caCompact<dim_type, value_type> compact(dims, chunkNums, maxLevel);

				compact.encode(bs, wtData, mmt);

				const unsigned char* converted = bs.data();
				for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
				{
					std::cout << std::dec << "[" << i << "]" << std::setfill('0') << std::setw(2) << std::right << std::hex << (short)converted[i] << std::endl;
					EXPECT_EQ((unsigned char)expected[i], (unsigned char)converted[i]);
				}
			}
		}
	}
}