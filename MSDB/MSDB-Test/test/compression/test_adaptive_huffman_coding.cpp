#include <pch.h>
#include <compression/adaptiveHuffmanCode.h>
#include <io/bitstream.h>

namespace msdb
{
namespace caDummy
{
TEST(adaptiveHuffmanCoding, encoding_decoding)
{
	const int dummySize = 1000;
	unsigned char dummy[dummySize];

	for(int i = 0; i < dummySize; ++i)
	{
		dummy[i] = (unsigned char)rand();
	}

	aHuffmanCoder<uint16_t> encoder(sizeof(unsigned char) * CHAR_BIT);
	bstream bs;

	for(int i = 0; i < dummySize; ++i)
	{
		encoder.encode(bs, dummy[i]);
	}

	std::cout << "Compressed size: " << bs.capacity() << std::endl;

	aHuffmanCoder<uint16_t> decoder(sizeof(unsigned char) * CHAR_BIT);
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
}	// caDummy
}	// msdb