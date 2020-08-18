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

	this->LIP_.clear();
	this->LIS_.clear();
	this->LIS_TYPE_.clear();
	this->LSP_.clear();

	coor init_coor(dSize);	// {0, 0, ...}
	size_t init_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		init_coor[d] = 0;
		init_num *= bandDims[d];
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
void spihtBlock::decode_init(dimension& bandSize)
{
	this->LIP_.clear();
	this->LIS_.clear();
	this->LIS_TYPE_.clear();
	this->LSP_.clear();

	size_t dSize = this->getDSize();
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
			} else
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