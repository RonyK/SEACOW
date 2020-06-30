#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/haar.h>
#include <util/math.h>
#include <list>
#include <memory>

namespace msdb
{
msdb::wavelet_encode_action::wavelet_encode_action()
{
	this->waveletName_ = "HaarSimple";
}

wavelet_encode_action::~wavelet_encode_action()
{
}

pArray wavelet_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray source = inputArrays[0];
	auto cItr = source->getChunkIterator();
	auto dSize = cItr.dSize();
	auto cSize = cItr.getSeqEnd();

	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	// Build wavelet_encode_array
	auto wArray = std::make_shared<wavelet_encode_array>(this->getArrayDesc(), maxLevel);
	// maxLevel value is checked in wavelet_encode_array constructor
	// which can be used for current array.
	maxLevel = wArray->getMaxLevel();

	for(chunkId id = 0; id < cSize; ++id, ++cItr)
	{
		auto cChunk = (*cItr);
		// --------------------
		// TODO::PARALLEL
		auto convertedChunkList = this->chunkEncode(wArray, (*cItr), w, maxLevel, q);
		// --------------------
		//for(auto c : convertedChunkList)
		//{
		//	c->print();
		//}
		//std::cout << "-----" << std::endl;

		wArray->insertChunk(convertedChunkList.begin(), convertedChunkList.end());
	}

	return wArray;
}

std::list<pChunk> wavelet_encode_action::chunkEncode(pArray wArray, pChunk sourceChunk,
													 pWavelet w, size_t maxLevel, pQuery q)
{
	std::list<pChunk> chunks;
	chunks.push_back(sourceChunk);

	for(size_t level = 0; level <= maxLevel; level++)
	{
		auto l = this->waveletLevelEncode(chunks.front(), w, q);
		chunks.pop_front();
		chunks.insert(chunks.begin(), l.begin(), l.end());
	}

	// Set new chunk IDs
	chunkId newId = sourceChunk->getId() * pow(pow(2, maxLevel + 1), wArray->getDesc()->dimDescs_.size());
	chunkId band = 0;
	for (auto c : chunks)
	{
		c->setId(newId + band);
		band++;
	}

	return chunks;
}

std::list<pChunk> wavelet_encode_action::waveletLevelEncode(pChunk wChunk, pWavelet w, pQuery q)
{
	std::list<pChunk> levelChunks;
	levelChunks.push_back(wChunk);
	size_t numOfLevelChunks = 1;

	for(dimensionId d = 0; d < this->aDesc_->dimDescs_.size(); ++d)
	{
		for(size_t i = 0; i < numOfLevelChunks; i++)
		{
			pChunk chunkBands = levelChunks.front();
			levelChunks.pop_front();
			std::list<pChunk> newChunkBands;

			switch(wChunk->getDesc()->attrDesc_->type_)
			{
			case eleType::CHAR:
				newChunkBands = waveletTransform<char>(chunkBands, w, d, q);
				break;
			case eleType::INT8:
				newChunkBands = waveletTransform<int8_t>(chunkBands, w, d, q);
				break;
			case eleType::INT16:
				newChunkBands = waveletTransform<int16_t>(chunkBands, w, d, q);
				break;
			case eleType::INT32:
				newChunkBands = waveletTransform<int32_t>(chunkBands, w, d, q);
				break;
			case eleType::INT64:
				newChunkBands = waveletTransform<int64_t>(chunkBands, w, d, q);
				break;
			case eleType::UINT8:
				newChunkBands = waveletTransform<uint8_t>(chunkBands, w, d, q);
				break;
			case eleType::UINT16:
				newChunkBands = waveletTransform<uint16_t>(chunkBands, w, d, q);
				break;
			case eleType::UINT32:
				newChunkBands = waveletTransform<uint32_t>(chunkBands, w, d, q);
				break;
			case eleType::UINT64:
				newChunkBands = waveletTransform<uint64_t>(chunkBands, w, d, q);
				break;
			case eleType::DOUBLE:
				newChunkBands = waveletTransform<double>(chunkBands, w, d, q);
				break;
			default:
				_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
			}

			levelChunks.insert(
				levelChunks.end(), newChunkBands.begin(), newChunkBands.end());
		}

		numOfLevelChunks *= 2;
	}

	return levelChunks;
}

//std::list<pChunk> wavelet_encode_action::waveletTransform(pChunk inChunk, pWavelet w, dimensionId basisDim, pQuery q)
//{
//	// Setting chunkDesc for band chunk
//	auto bandDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
//	bandDesc->setDim(basisDim, intDivCeil(bandDesc->dims_[basisDim], 2));
//
//	// Make chunk
//	pChunk approximateChunk = std::make_shared<chunk>(bandDesc);
//	pChunk detailChunk = std::make_shared<chunk>(bandDesc);
//
//	approximateChunk->materialize();
//	detailChunk->materialize();
//
//	// Get Iterator for each chunk
//	auto iit = inChunk->getItemIterator();
//	auto ait = approximateChunk->getItemIterator();
//	auto dit = detailChunk->getItemIterator();
//
//	iit.setBasisDim(basisDim);
//	ait.setBasisDim(basisDim);
//	dit.setBasisDim(basisDim);
//
//	iit.moveToStart();
//	ait.moveToStart();
//	dit.moveToStart();
//
//	// Iterate data
//	size_t length = inChunk->getDesc()->dims_[basisDim];
//	size_t rows = intDivCeil(inChunk->numCells(), length);
//	
//	for(size_t r = 0; r < rows; r++)
//	{
//		for(size_t i = 0; i < length; i += 2)
//		{
//			for(size_t j = 0; (j < w->c_) && (i + j < length); j++)
//			{
//				auto ele = iit.getAt(j);
//				//*ait += w->h_0[j] * iit.getAt(j);
//				//*dit += w->g_0[j] * iit.getAt(j);
//			}
//
//			++ait;
//			++dit;
//		}
//	}
//
//	return std::list<pChunk>({ approximateChunk, detailChunk });
//}

const char* msdb::wavelet_encode_action::name()
{
	return "wavelet_encode";
}
}

