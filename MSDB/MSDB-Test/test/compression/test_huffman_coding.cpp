#include <pch.h>
#include <io/bitstream.h>
#include <compression/huffmanCode.h>
#include <compression/fixedHuffmanCoder.h>

namespace msdb
{
namespace caDummy
{

void compareNode(huffmanCoder<uint16_t, uint8_t>::huffmanNode* a, huffmanCoder<uint16_t, uint8_t>::huffmanNode* b)
{
	if (a->left_ == nullptr)
	{
		if (b->left_)
		{
			std::cout << "error" << std::endl;
		}

		assert(a->symbol_ == b->symbol_ && "symbol different");
		assert(a->code_ == b->code_ && "code different");
		assert(a->codeLen_ == b->codeLen_ && "code len differnet");
	}

	if (a->left_)
	{
		if (b->left_ == nullptr)
		{
			std::cout << "error" << std::endl;
		}

		compareNode(a->left_, b->left_);
		compareNode(a->right_, b->right_);
	}
}

TEST(huffman_coding, huffman_coding)
{
	const int dummySize = 10000;
	unsigned char dummy[dummySize];
	unsigned char result[dummySize];

	for (int i = 0; i < dummySize; ++i)
	{
		dummy[i] = (unsigned char)rand();
	}

	huffmanCoder<uint16_t, uint8_t> coder;
	bstream bs;
	coder.encode(bs, (uint8_t*)dummy, dummySize);

	unsigned char out[dummySize] = {0};
	auto compressedSize = bs.capacity();
	std::cout << compressedSize << std::endl;

	huffmanCoder<uint16_t, uint8_t> decoder;
	
	decoder.decode((uint8_t*)result, dummySize, (uint8_t*)bs.data(), bs.capacity());

	compareNode(coder.root_, decoder.root_);

	for (int i = 0; i < dummySize; ++i)
	{
		if (dummy[i] != result[i])
		{
			std::cout << static_cast<uint32_t>(dummy[i]) << " <> " << static_cast<uint32_t>(result[i]);
		}

	}
}

TEST(huffman_coding, fixed_2byte_huffman_coding)
{
	const static int const size = 10;

	srand(0);
	uint16_t arr[size];
	for (int i = 0; i < size; ++i)
	{
		arr[i] = rand() % (int)pow(2, 9);
	}

	uint8_t* temp = (uint8_t*)arr;

	bstream bs;
	{
		auto coder = fixedHuffmanCoder<9, uint8_t>::instance();
		coder->encode(bs, arr, sizeof(uint16_t) * size);
	}

	uint16_t decoded[size];
	{
		auto coder = fixedHuffmanCoder<9, uint8_t>::instance();
		coder->decode(decoded, sizeof(uint16_t) * size, bs);
	}

	for (int i = 0; i < size; ++i)
	{
		assert(arr[i] == decoded[i]);
	}


}	// TEST
}	// dummy
}	// msdb