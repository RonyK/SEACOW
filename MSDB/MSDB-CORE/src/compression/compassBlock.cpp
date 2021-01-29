#include <stdafx.h>
#include <compression/compassBlock.h>

namespace msdb
{
compassBlock::compassBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
compassBlock::~compassBlock()
{
}

void compassBlock::serializePositional(bstream& bs, std::vector<position_t>& positional)
{
	position_t maxGap = 0;
	position_t prevP = positional[0];
	for(auto p: positional)
	{
		if(p - prevP > maxGap)
		{
			maxGap = p - prevP;
		}
	}

	bs << setw(CHAR_BIT) << msb<position_t>(positional[0]) << msb<position_t>(maxGap);
	bs << setw(msb<position_t>(positional[0])) << positional[0];	// set first position
	bs << setw(msb<position_t>(maxGap));

	prevP = 0;
	for (auto p : positional)
	{
		assert(p == 0 || p > prevP && "check positional vector is ordered");
		bs << p - prevP;
	}
	bs << 0;
}

void compassBlock::deserializePositional(bstream& bs, std::vector<position_t>& positional)
{
	bit_cnt_type bMaxGap = 0;
	bit_cnt_type bFirstPosition = 0;

	position_t p = 0;
	position_t prevP = 0;

	bs >> setw(CHAR_BIT) >> bFirstPosition >> bMaxGap;
	bs >> setw(bFirstPosition) >> p;		// get first position
	bs >> setw(bMaxGap);

	do
	{
		positional.push_back(p + prevP);
		bs >> p;
		prevP = p;

	} while (p != 0);
}

void compassBlock::setNumBins_(size_t numBins)
{
	this->numBins_ = numBins;
}
}	// msdb
