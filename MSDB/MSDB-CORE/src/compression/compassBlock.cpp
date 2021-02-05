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
		prevP = p;
	}

	auto bFirstPosition = std::max({ msb<position_t>(positional[0]), (unsigned char)1 });
	auto bMaxGap = std::max({ msb<position_t>(maxGap), (unsigned char)1 });

	bs << setw(CHAR_BIT) << bFirstPosition << bMaxGap;
	bs << setw(bFirstPosition) << positional[0];	// set first position
	bs << setw(bMaxGap);

	BOOST_LOG_TRIVIAL(debug) << "bFP: " << static_cast<int64_t>(bFirstPosition) << ", bMG: "  << static_cast<int64_t>(bMaxGap);

	prevP = 0;
	size_t numPositions = positional.size();
	for (size_t i = 1; i < numPositions; ++i)
	{
		position_t p = positional[i];
		assert((p == 0 || (p > prevP && p - prevP > 0)) && "check positional vector is ordered");
		bs << (p - prevP);
		prevP = p;
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
	positional.push_back(p);
	p = prevP;
	BOOST_LOG_TRIVIAL(debug) << "bFP: " << static_cast<int64_t>(bFirstPosition) << ", bMG: " << static_cast<int64_t>(bMaxGap);

	do
	{
		bs >> p;
		if(!p)
		{
			break;
		}
		positional.push_back(p + prevP);
		prevP += p;

	} while (true);
}

void compassBlock::setNumBins(size_t numBins)
{
	this->numBins_ = numBins;
}
}	// msdb