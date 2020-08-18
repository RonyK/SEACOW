#include <pch.h>
#include <compression/testCompression.h>

#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_compression_action.h>
#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
pArray wavelet_encode(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 0;
	std::shared_ptr<wavelet_encode_plan> wePlan;
	std::shared_ptr<wavelet_encode_action> weAction;
	pQuery weQuery;
	getWaveletEncode(sourceArr[0]->getDesc(), level, wePlan, weAction, weQuery);

	return weAction->execute(sourceArr, weQuery);
}

void wavelet_encode_check(pArray arr)
{
	//////////////////////////////
	// Check Encoded Result
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Check Encoded Result" << std::endl;
		value_type expected[dataLength] = { 0 };
		getWTChunkDummy(expected, dataLength);
		size_t cId = 0;
		size_t numItems = 0;
		for (size_t i = 0; i < dataLength; )
		{
			auto chk = arr->getChunk(cId);
			auto bItr = chk->getBlockIterator();

			while(!bItr->isEnd())
			{
				auto bit = (**bItr)->getItemIterator();

				while(!bit->isEnd())
				{
					std::cout << "[" << bit->coor()[0] << ", " << bit->coor()[1] << "(" << bit->seqPos() << ")]" << std::endl;
					std::cout <<  static_cast<int>((**bit).getChar()) << " <> " << static_cast<int>(expected[i]) << std::endl;
					EXPECT_EQ(ROUNDING((**bit).getChar(), 6), ROUNDING(expected[i], 6));

					++(*bit);
					++i;
					++numItems;
				}

				++(*bItr);
			}
			++cId;
		}

		EXPECT_EQ(numItems, dataLength);
	}
}

pArray wavelet_decode(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 0;
	std::shared_ptr<wavelet_decode_plan> wdPlan;
	std::shared_ptr<wavelet_decode_action> wdAction;
	pQuery wdQuery;
	getWaveletDecode(sourceArr[0]->getDesc(), level, wdPlan, wdAction, wdQuery);

	return wdAction->execute(sourceArr, wdQuery);
}

void wavelet_decode_check(pArray arr)
{
	std::cout << "##############################" << std::endl;
	std::cout << "Check Decoded Result" << std::endl;
	value_type deExpected[dataLength] = { 0 };
	getChunkDummy(deExpected, dataLength);
	size_t cId = 0;
	for (size_t i = 0; i < dataLength;)
	{
		auto it = arr->getChunk(cId)->getItemIterator();
		assert(it->getCapacity() == chunkDims[0] * chunkDims[1]);

		for (size_t j = 0; j < it->getCapacity(); ++j, ++i, ++(*it))
		{
			std::cout << static_cast<int>((**it).getChar()) << " <> " << static_cast<int>(deExpected[i]) << std::endl;
			EXPECT_EQ(ROUNDING((**it).getChar(), 6), ROUNDING(deExpected[i], 6));
		}
		++cId;
	}
}

pArray se_compression(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<se_compression_plan> sePlan;
	std::shared_ptr<se_compression_action> seAction;
	pQuery seQuery;
	getSeCompression(sourceArr[0]->getDesc(), sePlan, seAction, seQuery);

	auto afterArray = seAction->execute(sourceArr, seQuery);

	return afterArray;
}

pArray se_decompression(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<se_decompression_plan> sePlan;
	std::shared_ptr<se_decompression_action> seAction;
	pQuery seQuery;
	getSeDecompression(sourceArr[0]->getDesc(), sePlan, seAction, seQuery);

	auto afterArray = seAction->execute(sourceArr, seQuery);

	return afterArray;
}
void se_decompression_check(pArray arr)
{

}

pArray spiht_encode(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<spiht_encode_plan> spihtPlan;
	std::shared_ptr<spiht_encode_action> spihtAction;
	pQuery spihtQuery;
	getSPIHTEncode(sourceArr[0]->getDesc(), spihtPlan, spihtAction, spihtQuery);

	auto afterArray = spihtAction->execute(sourceArr, spihtQuery);

	return afterArray;
}
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
pArray wavelet_encode(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 0;
	std::shared_ptr<wavelet_encode_plan> wePlan;
	std::shared_ptr<wavelet_encode_action> weAction;
	pQuery weQuery;
	getWaveletEncode(sourceArr[0]->getDesc(), level, wePlan, weAction, weQuery);

	return weAction->execute(sourceArr, weQuery);
}

pArray wavelet_decode(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 0;
	std::shared_ptr<wavelet_decode_plan> wdPlan;
	std::shared_ptr<wavelet_decode_action> wdAction;
	pQuery wdQuery;
	getWaveletDecode(sourceArr[0]->getDesc(), level, wdPlan, wdAction, wdQuery);

	return wdAction->execute(sourceArr, wdQuery);
}

pArray se_compression(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<se_compression_plan> sePlan;
	std::shared_ptr<se_compression_action> seAction;
	pQuery seQuery;
	getSeCompression(sourceArr[0]->getDesc(), sePlan, seAction, seQuery);

	auto afterArray = seAction->execute(sourceArr, seQuery);

	return afterArray;
}

}	// data2D_star1024x1024
}	// caDummy
}	// msdbk
