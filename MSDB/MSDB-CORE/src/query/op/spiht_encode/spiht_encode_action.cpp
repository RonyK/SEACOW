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

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	auto source = inputArrays[0];
	auto wArray = std::static_pointer_cast<wavelet_encode_array>(source);

	auto chunkItr = wArray->getChunkIterator();
	auto itemItr = (*chunkItr)->getItemIterator();

	// TODO: !(sign bit) abs

	auto dSize = chunkItr.dSize();									// dimension size
	auto cSize = wArray->getDesc()->dimDescs_.getChunkDims();		// chunk size
	auto max_level = wArray->getMaxLevel();							// max level
	std::vector<size_t> bandSize(dSize);							// band size in max level(?)
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		bandSize[d] = (size_t)(cSize[d] / pow(2, max_level + 1));
	}

	this->encode_init(dSize, bandSize);

	auto maxStep = wArray->getDesc()->attrDescs_[0]->typeSize_ * 8;		// bit num
	for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
	{
		size_t LSP_size = this->LSP_.size();
		switch (wArray->getDesc()->attrDescs_[0]->type_)
		{
		case eleType::INT32:
			this->encode_sigpass<int32_t>(dSize, cSize, bandSize, itemItr, curStep, maxStep);
			this->encode_refinepass<int32_t>(itemItr, curStep, maxStep, LSP_size);
		default:
			break;
		}
		
	}
	return source;
}

void spiht_encode_action::encode_init(size_t dSize, std::vector<size_t> bandSize)
{
	this->LIP_.clear();
	this->LIS_.clear();
	this->LIS_TYPE_.clear();
	this->LSP_.clear();
	
	coor init(dSize);	// {0, 0, ...}
	size_t init_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init[d] = 0;
		init_num *= bandSize[d];
	}
	
	for (size_t i = 0; i < init_num; i++)
	{
		this->LIP_.push_back(init);	

		bool haveChild = false;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			if (init[d] & 0x1)		// odd coordinate
			{
				haveChild = true;
				break;
			}
		}

		if (haveChild)
		{
			this->LIS_.push_back(init);
			this->LIS_TYPE_.push_back('A');
		}


		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		{
			init[d] = init[d] + 1;
			if (init[d] == bandSize[d])
			{
				init[d] = 0;
			}
			else
			{
				break;
			}
		}
	}
}

template<class Ty_>
void spiht_encode_action::encode_sigpass(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, chunkItemIterator itemItr, size_t curStep, size_t maxStep)
{
	Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
	Ty_ stepBit = (Ty_)0x1 << (maxStep - 2 - curStep);

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
		char LIS_type = this->LIS_TYPE_.front();
		this->LIS_.pop_front();
		this->LIS_TYPE_.pop_front();

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
						child_coor[d] = child_coor[d] - 1;
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
							child_coor[d] = child_coor[d] - 1;
						}
					}
				}

				bool haveGrand = true;
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
							grand_coor[d] = grand_coor[d] - 1;
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
						child_coor[d] = child_coor[d] - 1;
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
							child_coor[d] = child_coor[d] - 1;
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
void spiht_encode_action::encode_refinepass(chunkItemIterator itemItr, size_t curStep, size_t maxStep, size_t LSP_size)
{
	Ty_ stepBit = (Ty_)0x1 << (maxStep - 2 - curStep);
	std::list<coor> TMP_ = this->LSP_;

	for (size_t i = 0; i < LSP_size; i++)
	{
		coor LSP_coor = TMP_.front();
		TMP_.pop_front();

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