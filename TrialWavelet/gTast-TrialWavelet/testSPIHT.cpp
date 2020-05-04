#include "pch.h"
#include "dummy.h"
#include "../TrialWavelet/spiht.h"

namespace caWavelet
{
	namespace caDummy
	{
		namespace data2D_s4x4
		{
			TEST(caSPIHT, d2_s4x4_spiht)
			{
				//////////////////////////////
				// Build Dummy Data
				value_type data[dummyDataLength];
				value_type wtData[dummyDataLength];
				value_type expected[dummyDataLength];

				getDummy(data, dummyDataLength);
				getWTDummy(wtData, dummyDataLength);
				getEXDummy(expected, dummyDataLength);
				//////////////////////////////

				caSPIHT<dim_type, value_type> spiht;
				std::vector<std::bitset<BS_SIZE_>> output;
				spiht.encode(wtData, output, dims, maxLevel);

				for (size_t i = 0; i < output.size(); i++)
				{
					std::cout << output[i] << std::endl;
				}
			}
		}
	}
}
