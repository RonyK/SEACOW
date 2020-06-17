#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/haar.h>
#include <list>
#include <memory>

namespace msdb
{
msdb::wavelet_encode_action::wavelet_encode_action()
{
}

pArray wavelet_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray source = inputArrays[0];
	auto cItr = source->getIterator();
	auto dSize = cItr.dSize();
	auto cSize = cItr.getSeqEnd();

	//// Get parameters
	//pStableElement ele = std::dynamic_pointer_cast<stableElement>(this->params_[1]->getParam());
	//eleDefault maxLevel;
	//ele->getData(&maxLevel);
	//pWavelet w = std::make_shared<wavelet>("Haar");

	//// Build wavelet_encode_array
	//auto wArray = std::shared_ptr<wavelet_encode_array>(new wavelet_encode_array(this->getArrayDesc()));
	//for(chunkId id = 0; id < cSize; id++)
	//{
	//	// --------------------
	//	// TODO::PARALLEL
	//	auto l = this->chunkEncode(wArray, (*cItr), q, w, maxLevel);
	//	// --------------------
	//	wArray->insertChunk(l.begin(), l.end());
	//	
	//	++cItr;
	//}

	//return wArray;

	return inputArrays[0];
}

std::list<pChunk> wavelet_encode_action::chunkEncode(pArray wArray, pChunk targetChunk,
										pQuery q, pWavelet w, size_t maxLevel)
{
	std::list<pChunk> chunks;


	for(size_t level = 0; level < maxLevel; level++)
	{
		auto l = this->waveletLevelEncode(chunks.front(), q, w);
		chunks.pop_front();
		chunks.insert(chunks.begin(), l.begin(), l.end());
	}

	return chunks;
}

std::list<pChunk> wavelet_encode_action::waveletLevelEncode(pChunk wChunk, 
															pQuery q, pWavelet w)
{
	std::list<pChunk> levelChunks;
	levelChunks.push_back(wChunk);
	size_t numOfLevelChunks = 1;

	for(dimensionId d = this->aDesc_->dimDescs_.size() - 1; d != INVALID_DIMENSION_ID; d--)
	{
		for(size_t i = 0; i < numOfLevelChunks; i++)
		{
			pChunk chunkBands = levelChunks.front();
			levelChunks.pop_front();

			std::list<pChunk> newChunkBands = this->waveletTransform(chunkBands, q, w);

			levelChunks.insert(
				levelChunks.end(), newChunkBands.begin(), newChunkBands.end());
		}

		numOfLevelChunks *= 2;
	}

	return levelChunks;
}

std::list<pChunk> wavelet_encode_action::waveletTransform(pChunk inChunk, pQuery q, pWavelet w)
{
	pChunk approximateChunk = std::make_shared<chunk>(inChunk->getDesc());
	pChunk detailChunk = std::make_shared<chunk>(inChunk->getDesc());
	//outChunk->materialize(inChunk->getDesc()->mSize_);




	return std::list<pChunk>();
}

const char* msdb::wavelet_encode_action::name()
{
	return "wavelet_encode";
}
}

