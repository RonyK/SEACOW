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

	int spX;
	int spY;
	int width;
	int height;

	while (1)
	{
		spX = rand() % (size_t)(dimX - dimX * selectivity / 100.0);
		spY = rand() % (size_t)(dimY - dimY * selectivity / 100.0);

		for (int i = 0; i < 3; i++)
		{
			width = rand() % (dimX - spX);
			if (width > selectedArea)
				continue;

			height = selectedArea / width;

			if (spX + width < dimX && spY + height < dimY)
				break;
		}

		if (spX + width < dimX && spY + height < dimY)
			break;
	}

	int epX = spX + width;
	int epY = spY + height;
	int cal = (epX - spX) * (epY - spY);

	coor sp = { spX, spY };
	coor ep = { spX + width, spY + height };

	return coorRange(sp, ep);
}
}	// caDummy
}	// msdb