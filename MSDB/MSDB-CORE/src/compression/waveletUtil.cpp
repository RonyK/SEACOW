#include <stdafx.h>
#include <compression/waveletUtil.h>

namespace msdb
{
coorRange getBandRange(const size_t bandId, const coor& bandDims)
{
	size_t dSize = bandDims.size();
	coor bandSp(dSize);
	coor bandEp(bandDims);
	
	for (dimensionId d = 0; d < dSize; ++d)
	{
		if (bandId & (0x1 << (dSize - d - 1)))
		{
			bandSp[d] += bandDims[d];
			bandEp[d] += bandDims[d];
		}
	}
	return coorRange(bandSp, bandEp);
}
}		// msdb