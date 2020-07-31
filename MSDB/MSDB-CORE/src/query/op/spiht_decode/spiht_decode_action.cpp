#include <op/spiht_decode/spiht_decode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
spiht_decode_action::spiht_decode_action()
{
}

spiht_decode_action::~spiht_decode_action()
{
}

const char* spiht_decode_action::name()
{
	return "spiht_decode";
}

pArray spiht_decode_action::execute(std::vector<pArray>& outputArrays, pQuery q)
{
	// TODO: code setting
	auto source = outputArrays[0];
	auto wArray = std::static_pointer_cast<wavelet_encode_array>(source);
	auto chunkItr = wArray->getChunkIterator();
	while (!chunkItr.isEnd())
	{
		auto itemItr = (*chunkItr)->getItemIterator();
		auto dSize = chunkItr.dSize();									// dimension size
		auto cSize = wArray->getDesc()->dimDescs_.getChunkDims();		// chunk size
		auto max_level = wArray->getMaxLevel();							// max level
		std::vector<size_t> bandSize(dSize);							// band size in max level(?)
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			bandSize[d] = (size_t)(cSize[d] / pow(2, max_level + 1));
		}

		this->decode_init(dSize, bandSize);

		switch ((*chunkItr)->getDesc()->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->decode_progress<char>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT8:
			this->decode_progress<int8_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT16:
			this->decode_progress<int16_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT32:
			this->decode_progress<int32_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT64:
			this->decode_progress<int64_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT8:
			this->decode_progress<uint8_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT16:
			this->decode_progress<uint16_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT32:
			this->decode_progress<uint32_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT64:
			this->decode_progress<uint64_t>(dSize, cSize, bandSize, itemItr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
		++chunkItr;
	}

	return source;
}

void spiht_decode_action::decode_init(size_t dSize, std::vector<size_t> bandSize)
{
	this->LIP_.clear();
	this->LIS_.clear();
	this->LIS_TYPE_.clear();
	this->LSP_.clear();

	coor init_coor(dSize);	// {0, 0, ...}
	size_t init_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init_coor[d] = 0;
		init_num *= bandSize[d];
	}

	for (size_t i = 0; i < init_num; i++)
	{
		this->LIP_.push_back(init_coor);

		bool haveChild = false;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			if (init_coor[d] & 0x1)		// odd coordinate
			{
				haveChild = true;
				break;
			}
		}

		if (haveChild)
		{
			this->LIS_.push_back(init_coor);
			this->LIS_TYPE_.push_back('A');
		}


		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		{
			init_coor[d] = init_coor[d] + 1;
			if (init_coor[d] == bandSize[d])
			{
				init_coor[d] = 0;
			}
			else
			{
				break;
			}
		}
	}
}
}