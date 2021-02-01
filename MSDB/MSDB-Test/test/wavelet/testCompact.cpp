//#include <pch.h>
//#include <compression/compact.h>
//
//namespace msdb
//{
//	namespace caDummy
//	{
//		namespace data2D_sc4x4
//		{
//			TEST(compact, d2_s4x4_compact)
//			{
//				//////////////////////////////
//				// Build Dummy Data
//				value_type data[dataLength];
//				value_type wtData[dataLength];
//				value_type expected[dataLength];
//
//				getDummy(data, dataLength);
//				getWTDummy(wtData, dataLength);
//				getExDummy(expected, dataLength);
//				//////////////////////////////
//
//				mmt_old<dim_type, value_type> mmt(dims, chunkDims, maxLevel);
//				mmt.build(data, dataLength);
//
//				bstream bs;
//				caCompact<dim_type, value_type> compact(dims, chunkNums, maxLevel);
//
//				compact.encode(bs, wtData, mmt);
//
//				const unsigned char* converted = bs.data();
//				for (size_t i = 0; i < (bs.size() + CHAR_BIT - 1) / CHAR_BIT; i++)
//				{
//					std::cout << std::dec << "[" << i << "]" << std::setfill('0') << std::setw(2) << std::right << std::hex << (short)converted[i] << std::endl;
//					EXPECT_EQ((unsigned char)expected[i], (unsigned char)converted[i]);
//				}
//			}
//		}
//	}
//}