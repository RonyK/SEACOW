#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
spiht_encode_action::spiht_encode_action()
{
}

spiht_encode_action::~spiht_encode_action()
{
}

const char* spiht_encode_action::name()
{
	return "spiht_encode";
}

std::list<int> spiht_encode_action::getCode()
{
	return this->code;
}

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	auto source = inputArrays[0];
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

		this->encode_init(dSize, bandSize);

		switch ((*chunkItr)->getDesc()->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->encode_progress<char>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT8:
			this->encode_progress<int8_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT16:
			this->encode_progress<int16_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT32:
			this->encode_progress<int32_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::INT64:
			this->encode_progress<int64_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT8:
			this->encode_progress<uint8_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT16:
			this->encode_progress<uint16_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT32:
			this->encode_progress<uint32_t>(dSize, cSize, bandSize, itemItr);
			break;
		case eleType::UINT64:
			this->encode_progress<uint64_t>(dSize, cSize, bandSize, itemItr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}


		++chunkItr;
	}

	return source;
}

void spiht_encode_action::encode_init(size_t dSize, std::vector<size_t> bandSize)
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

template<class Ty_>
void spiht_encode_action::encode_progress(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, chunkItemIterator itemItr)
{
	size_t maxStep = sizeof(Ty_) * 8;
	Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
	Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);
	// abs
	coor init_coor(dSize);	// {0, 0, ...}
	size_t abs_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init_coor[d] = 0;
		abs_num *= cSize[d];
	}
	
	for (size_t i = 0; i < abs_num; i++)
	{
		itemItr.moveTo(init_coor);
		auto data = (*itemItr).get<Ty_>();
		if (data & signBit)
		{
			data = data * -1;
			data = data ^ signBit;
			(*itemItr).set<Ty_>(data);
		}

		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		{
			init_coor[d] = init_coor[d] + 1;
			if (init_coor[d] == cSize[d])
			{
				init_coor[d] = 0;
			}
			else
			{
				break;
			}
		}
	}

	for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
	{
		size_t LSP_size = this->LSP_.size();
		this->encode_sigpass<Ty_>(dSize, cSize, bandSize, itemItr, signBit, stepBit);
		this->encode_refinepass<Ty_>(itemItr, stepBit, LSP_size);
		stepBit = stepBit >> 1;
	}


}

template<class Ty_>
void spiht_encode_action::encode_sigpass(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, 
	chunkItemIterator itemItr, Ty_ signBit, Ty_ stepBit)
{
	// LIP
	size_t LIP_size = this->LIP_.size();
	for (size_t i = 0; i < LIP_size; i++)
	{
		auto LIP_coor = this->LIP_.front();
		this->LIP_.pop_front();
		itemItr.moveTo(LIP_coor);
		auto LIP_data = (*itemItr).get<Ty_>();
		
		if (LIP_data & stepBit)
		{
			this->code.push_back(1);	// TODO
			if (LIP_data & signBit)  // -
			{
				this->code.push_back(1);	// TODO
			}
			else  // +
			{
				this->code.push_back(0);	// TODO
			}
			this->LSP_.push_back(LIP_coor);
		}
		else
		{
			this->code.push_back(0);	// TODO
			this->LIP_.push_back(LIP_coor);
		}

	}

	// LIS
	std::list<coor> TMP_;
	std::list<char> TMP_TYPE_;
	coor child_coor(dSize);
	coor grand_coor(dSize);
	while (this->LIS_.size() != 0)
	{
		coor LIS_coor = this->LIS_.front();
		this->LIS_.pop_front();
		itemItr.moveTo(LIS_coor);
		auto LIS_data = (*itemItr).get<Ty_>();

		// set child_coor
		bool firstBand = true;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			if (LIS_coor[d] >= bandSize[d])
			{
				firstBand = false;
				break;
			}
		}

		if (firstBand)
		{
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				child_coor[d] = (LIS_coor[d] / 2) * 2;
				if (LIS_coor[d] & 0x1)
				{
					child_coor[d] = child_coor[d] + bandSize[d];
				}
			}
		}
		else    // other level bands
		{
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				child_coor[d] = LIS_coor[d] * 2;
			}
		}
		coor temp_coor = child_coor;

		char LIS_type = this->LIS_TYPE_.front();
		this->LIS_TYPE_.pop_front();
		if (LIS_type == 'A')	// type A
		{	
			bool oneFlag = false;
			for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
			{
				itemItr.moveTo(child_coor);
				auto child_data = (*itemItr).get<Ty_>();

				if (child_data & stepBit)
				{
					oneFlag = true;
					break;
				}

				for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
				{
					child_coor[d] = child_coor[d] + 1;
					if (child_coor[d] & 0x1)
					{
						break;
					}
					else
					{
						child_coor[d] = child_coor[d] - 2;
					}
				}
			}
			
			if (oneFlag)
			{
				this->code.push_back(1);	// TODO

				child_coor = temp_coor;
				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
				{
					itemItr.moveTo(child_coor);
					auto child_data = (*itemItr).get<Ty_>();

					if (child_data & stepBit)
					{
						this->code.push_back(1);	// TODO
						if (child_data & signBit)  // -
						{
							this->code.push_back(1);	// TODO
						}
						else  // +
						{
							this->code.push_back(0);	// TODO
						}
						this->LSP_.push_back(child_coor);
					}
					else
					{
						this->code.push_back(0);	// TODO
						this->LIP_.push_back(child_coor);
					}

					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
					{
						child_coor[d] = child_coor[d] + 1;
						if (child_coor[d] & 0x1)
						{
							break;
						}
						else
						{
							child_coor[d] = child_coor[d] - 2;
						}
					}
				}

				bool haveGrand = true;
				child_coor = temp_coor;
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					if (child_coor[d]*2 >= cSize[d])
					{
						haveGrand = false;
						break;
					}
				}

				if (haveGrand)
				{
					this->LIS_.push_back(LIS_coor);
					this->LIS_TYPE_.push_back('B');
				}
			}
			else
			{
				this->code.push_back(0);	// TODO
				TMP_.push_back(LIS_coor);
				TMP_TYPE_.push_back(LIS_type);
			}
		}
		else    // type B
		{
			child_coor = temp_coor;
			bool oneFlag = false;
			for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
			{
				for (int d = (int)dSize - 1; d >= 0; d--)
				{
					grand_coor[d] = child_coor[d] * 2;
				}

				for (size_t j = 0; j < (size_t)pow(2, dSize); j++)
				{
					itemItr.moveTo(grand_coor);
					auto grand_data = (*itemItr).get<Ty_>();

					if (grand_data & stepBit)
					{
						oneFlag = true;
						break;
					}

					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
					{
						grand_coor[d] = grand_coor[d] + 1;
						if (grand_coor[d] & 0x1)
						{
							break;
						}
						else
						{
							grand_coor[d] = grand_coor[d] - 2;
						}
					}
				}

				if (oneFlag)
				{
					break;
				}

				for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
				{
					child_coor[d] = child_coor[d] + 1;
					if (child_coor[d] & 0x1)
					{
						break;
					}
					else
					{
						child_coor[d] = child_coor[d] - 2;
					}
				}
			}

			if (oneFlag)
			{
				this->code.push_back(1);	// TODO

				child_coor = temp_coor;
				for (size_t i = 0; i < (size_t)pow(2, dSize); i++)
				{
					this->LIS_.push_back(child_coor);
					this->LIS_TYPE_.push_back('A');

					for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
					{
						child_coor[d] = child_coor[d] + 1;
						if (child_coor[d] & 0x1)
						{
							break;
						}
						else
						{
							child_coor[d] = child_coor[d] - 2;
						}
					}
				}
			}
			else
			{
				this->code.push_back(0);	// TODO
				TMP_.push_back(LIS_coor);
				TMP_TYPE_.push_back(LIS_type);
			}
		}
	}
	this->LIS_ = TMP_;
	this->LIS_TYPE_ = TMP_TYPE_;
}

template<class Ty_>
void spiht_encode_action::encode_refinepass(chunkItemIterator itemItr, Ty_ stepBit, size_t LSP_size)
{
	for (size_t i = 0; i < LSP_size; i++)
	{
		coor LSP_coor = this->LSP_.front();
		this->LSP_.pop_front();
		itemItr.moveTo(LSP_coor);
		auto LSP_data = (*itemItr).get<Ty_>();

		if (LSP_data & stepBit)
		{
			this->code.push_back(1);	// TODO
		}
		else
		{
			this->code.push_back(0);	// TODO
		}
	}
}
}