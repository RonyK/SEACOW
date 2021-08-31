#include <pch.h>
#include <compression/adaptiveHuffmanCode.h>
#include <io/bitstream.h>
#include <limits>

namespace msdb
{
namespace core
{
namespace caDummy
{
TEST(adaptiveHuffmanCoding, encoding_decoding)
{
	const int dummySize = 10000;
	unsigned char dummy[dummySize];

	for(int i = 0; i < dummySize; ++i)
	{
		dummy[i] = (unsigned char)rand();
	}

	aHuffmanCoder<uint16_t, uint8_t> encoder(sizeof(uint8_t) * CHAR_BIT);
	bstream bs;

	for(int i = 0; i < dummySize; ++i)
	{
		encoder.encode(bs, dummy[i]);
	}

	std::cout << "Compressed size: " << bs.capacity() << std::endl;

	aHuffmanCoder<uint16_t, uint8_t> decoder(sizeof(uint8_t) * CHAR_BIT);
	unsigned char result[dummySize];

	for(int i = 0; i < dummySize; ++i)
	{
		result[i] = decoder.decode(bs);
	}

	for(int i = 0; i < dummySize; ++i)
	{
		EXPECT_EQ(dummy[i], result[i]);
	}
}

TEST(adaptiveHuffmanCoding, encoding_decoding_designate)
{
	const int dummySize = 320;
	unsigned char dummy[dummySize] = {
		5, 5, 7, 6, 8, 15, 8, 5, 6, 8, 8, 6, 5, 6, 5, 7, 8, 5, 5, 7, 7, 4, 4, 6, 7, 5, 6, 5, 7, 6, 6, 5, 4, 7, 7, 6, 6, 5, 6, 7, 7, 5, 7, 8, 8, 6, 7, 5, 4, 5, 7, 5, 6, 13, 11, 6, 5, 6, 6, 7, 7, 6, 6, 6,
		6, 7, 7, 6, 8, 7, 9, 6, 6, 7, 6, 6, 6, 7, 7, 7, 8, 6, 5, 7, 7, 7, 7, 6, 6, 4, 6, 7, 5, 5, 6, 13, 18, 7, 6, 6, 6, 7, 5, 6, 6, 7, 7, 8, 6, 7, 7, 6, 7, 6, 8, 6, 4, 12, 10, 5, 3, 6, 6, 5, 5, 7, 6, 9,
		6, 6, 7, 6, 10, 10, 8, 4, 7, 6, 5, 6, 8, 6, 6, 6, 8, 7, 8, 5, 6, 5, 8, 6, 5, 8, 5, 6, 5, 6, 6, 14, 19, 6, 7, 7, 6, 7, 6, 7, 6, 7, 9, 6, 8, 6, 7, 7, 6, 13, 11, 9, 7, 6, 8, 6, 6, 6, 7, 6, 5, 7, 9, 7,
		4, 6, 8, 7, 8, 8, 6, 4, 6, 8, 8, 5, 6, 7, 8, 8, 7, 7, 9, 6, 5, 5, 6, 7, 8, 7, 6, 5, 4, 7, 6, 5, 6, 8, 6, 6, 6, 7, 9, 6, 5, 6, 7, 6, 7, 7, 6, 7, 5, 19, 18, 8, 7, 7, 7, 6, 6, 7, 7, 7, 8, 6, 5, 5,
		5, 8, 8, 6, 5, 7, 7, 5, 4, 6, 6, 5, 8, 8, 8, 6, 6, 6, 7, 8, 6, 5, 6, 8, 6, 5, 8, 7, 5, 6, 7, 4, 7, 7, 5, 6, 8, 7, 6, 5, 6, 6, 5, 7, 7, 6, 5, 6, 6, 12, 25, 9, 4, 6, 7, 7, 5, 5, 6, 7, 8, 5, 5, 6
	};

	//for (int i = 0; i < dummySize; ++i)
	//{
	//	dummy[i] = (unsigned char)rand();
	//}

	aHuffmanCoder<uint16_t, uint8_t> encoder(sizeof(uint8_t) * CHAR_BIT);
	bstream bs;

	for (int i = 0; i < dummySize; ++i)
	{
		if(dummy[i] == 19)
		{
			std::cout << "i: " << i << std::endl;
		}
		encoder.encode(bs, dummy[i]);
	}

	std::cout << "Compressed size: " << bs.capacity() << std::endl;

	aHuffmanCoder<uint16_t, uint8_t> decoder(sizeof(uint8_t) * CHAR_BIT);
	unsigned char result[dummySize];

	for (int i = 0; i < dummySize; ++i)
	{
		result[i] = decoder.decode(bs);
	}

	for (int i = 0; i < dummySize; ++i)
	{
		EXPECT_EQ(dummy[i], result[i]);
	}
}
}	// caDummy
}		// core
}		// msdb