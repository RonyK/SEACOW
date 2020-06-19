#include <op/wavelet_encode/wavelet_encode_array.h>

namespace msdb
{
wavelet_encode_array::wavelet_encode_array(pArrayDesc desc, const size_t maxLevel)
	: arrayBase(desc)
{
	this->maxLevel_ = this->isMaxLevelAvailable(maxLevel);
	for(pDimensionDesc desc : this->desc_->dimDescs_)
	{
		desc->chunkSize_ /= pow(2, this->maxLevel_);
	}
}

size_t wavelet_encode_array::getMaxLevel()
{
	return this->maxLevel_;
}

bool wavelet_encode_array::isMaxLevelAvailable(const size_t maxLevel)
{
	size_t level = maxLevel;
	for (pDimensionDesc desc : this->desc_->dimDescs_)
	{
		while(desc->chunkSize_ < pow(2, level) && level > 0)
		{
			--level;
		}
		if (level == 0)
		{
			return 0;
		}
	}

	return level;
}
}