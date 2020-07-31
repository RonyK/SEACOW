#include <op/wavelet_decode/wavelet_decode_action.h>
#include <op/wavelet_decode/wavelet_decode_array.h>
#include <compression/wavelet.h>
#include <list>
#include <vector>
#include <memory>

namespace msdb
{
wavelet_decode_action::wavelet_decode_action()
{
	this->waveletName_ = "HaarSimple";
}

wavelet_decode_action::~wavelet_decode_action()
{
}

pArray wavelet_decode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray source = inputArrays[0];
	auto cItr = source->getChunkIterator();
	auto dSize = cItr.dSize();
	auto cSize = cItr.getSeqEnd();

	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	// Build wavelet_decode_array
	auto wdArray = std::make_shared<wavelet_decode_array>(this->getArrayDesc(), maxLevel);
	size_t chunkBundleSize = pow(2, dSize) + (pow(2, dSize) - 1) * maxLevel;		// TODO:: Modify if detail chunks are splited

	for(chunkId id = 0; id < cSize; id += chunkBundleSize)
	{
		std::list<pChunk> chunkBundle;

		for(chunkId iid = 0; iid < chunkBundleSize; ++iid)
		{
			pChunk pc = source->getChunk(id + iid);
			chunkBundle.push_back(pc);
		}

		switch(chunkBundle.front()->getDesc()->attrDesc_->type_)
		{
		case eleType::CHAR:
			wdArray->insertChunk(this->chunkDecode<char>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::INT8:
			wdArray->insertChunk(this->chunkDecode<int8_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::INT16:
			wdArray->insertChunk(this->chunkDecode<int16_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::INT32:
			wdArray->insertChunk(this->chunkDecode<int32_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::INT64:
			wdArray->insertChunk(this->chunkDecode<int64_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::UINT8:
			wdArray->insertChunk(this->chunkDecode<uint8_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::UINT16:
			wdArray->insertChunk(this->chunkDecode<uint16_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::UINT32:
			wdArray->insertChunk(this->chunkDecode<uint32_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		case eleType::UINT64:
			wdArray->insertChunk(this->chunkDecode<uint64_t>(wdArray, chunkBundle, w, maxLevel, q));
			break;
		//case eleType::DOUBLE:
		//	wdArray->insertChunk(this->chunkDecode<double>(wdArray, chunkBundle, w, maxLevel, q));
		//	break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	return wdArray;
}

const char* wavelet_decode_action::name()
{
	return "wavelet_decode";
}
}	// msdb