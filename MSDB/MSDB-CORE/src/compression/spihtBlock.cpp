#include <compression/spihtBlock.h>

namespace msdb
{
spihtBlock::spihtBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

spihtBlock::~spihtBlock()
{
}

void spihtBlock::encode_init(dimension& bandDims)
{
	size_t dSize = this->getDSize();
	dimension blockDims = this->desc_->dims_;

	this->ENLIP_.clear();
	this->ENLIS_.clear();
	this->ENLIS_TYPE_.clear();
	this->ENLSP_.clear();

	coor init_coor(dSize);	// {0, 0, ...}
	size_t init_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init_num *= bandDims[d];
	}

	for (size_t i = 0; i < init_num; i++)
	{
		// check child and set index
		size_t index = 0;
		size_t multiply = 1;
		bool haveChild = false;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			index += multiply * init_coor[d];
			multiply *= blockDims[d];
			if (init_coor[d] & 0x1)		// odd coordinate
			{
				haveChild = true;
			}
		}
		this->ENLIP_.push_back(index);

		if (haveChild)
		{
			this->ENLIS_.push_back(init_coor);
			this->ENLIS_TYPE_.push_back('A');
		}

		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		{
			init_coor[d] = init_coor[d] + 1;
			if (init_coor[d] == bandDims[d])
			{
				init_coor[d] = 0;
			} else
			{
				break;
			}
		}
	}
}

void spihtBlock::decode_init(dimension& bandDims)
{
	size_t dSize = this->getDSize();
	dimension blockDims = this->desc_->dims_;

	this->DELIP_.clear();
	this->DELIS_.clear();
	this->DELIS_TYPE_.clear();
	this->DELSP_.clear();

	coor init_coor(dSize);	// {0, 0, ...}
	size_t init_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init_num *= bandDims[d];
	}

	for (size_t i = 0; i < init_num; i++)
	{
		bool haveChild = false;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			if (init_coor[d] & 0x1)		// odd coordinate
			{
				haveChild = true;
				break;
			}
		}
		this->DELIP_.push_back(init_coor);

		if (haveChild)
		{
			this->DELIS_.push_back(init_coor);
			this->DELIS_TYPE_.push_back('A');
		}

		for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
		{
			init_coor[d] = init_coor[d] + 1;
			if (init_coor[d] == bandDims[d])
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

void spihtBlock::setMaxLevel(size_t maxLevel)
{
	this->maxLevel_ = maxLevel;
}
}	// msdb