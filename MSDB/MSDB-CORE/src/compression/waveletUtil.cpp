#include <compression/waveletUtil.h>

namespace msdb
{
coorRange getBandRange(size_t bandId, coor& bandDims)
{
	size_t dSize = bandDims.size();
	coor bandSp(dSize);
	coor bandEp(bandDims);
	
	for (dimensionId d = 0; d < dSize; ++d)
	{
		if (bandId & (0x1 << d))
		{
			bandSp[d] += bandDims[d];
			bandEp[d] += bandDims[d];
		}
	}
	return coorRange(bandSp, bandEp);
}
}		// msdb