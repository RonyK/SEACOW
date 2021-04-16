#include <pch.h>

#include <query/op/between/test_body_between.h>

namespace msdb
{
namespace caDummy
{
coorRange getRandomRange(const position_t dimX, const position_t dimY, const float selectivity)
{
	int area = dimX * dimY;
	float selectedArea = area * selectivity / 100.0;

	int spX = 0, spY = 0;
	int width = 1, height = 1;

	while (1)
	{
		spX = rand() % (size_t)(dimX - dimX * selectivity / 100.0);
		spY = rand() % (size_t)(dimY - dimY * selectivity / 100.0);

		for(int retry = 0; retry < 5; ++retry)
		{
			width = rand() % (dimX - spX);
			if (width > selectedArea)
				continue;

			height = selectedArea / width;

			if (width > 0 && height > 0 && (spX + width) < dimX && (spY + height) < dimY)
				break;
		}

		if (width > 0 && height > 0 && (spX + width) < dimX && (spY + height) < dimY)
			break;
	}

	int epX = spX + width;
	int epY = spY + height;
	int cal = (epX - spX) * (epY - spY);

	coor sp = { spY, spX };
	coor ep = { spY + height, spX + width };

	return coorRange(sp, ep);
}
}	// caDummy
}	// msdb