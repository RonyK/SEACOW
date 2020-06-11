#include <array/memChunk.h>
#include <system/exceptions.h>
#include <cassert>

namespace msdb
{
memChunk::memChunk()
{

}

bool memChunk::isCompressed() const
{
	return false;
}

memChunkIterator::memChunkIterator(void* data, eleType eType, const size_type dSize,
								   position_t* dims, dim_pointer csP)
	:itemItr(data, eType, dSize, dims)
{
	this->csP_ = new dim_type[dSize];
	this->memcpyDim(this->csP_, csP);
}

memChunkIterator::coordinate_type memChunkIterator::coorOut2In(coordinate_type& out)
{
	coordinate_type in(this->dSize());
	for(dimensionId d = 0; d < this->dSize(); d++)
	{
		in[d] = out[d] - this->csP_[d];
	}

	return in;
}

memChunkIterator::coordinate_type memChunkIterator::coorIn2Out(coordinate_type& in)
{
	coordinate_type out(this->dSize());
	for (dimensionId d = 0; d < this->dSize(); d++)
	{
		out[d] = in[d] + this->csP_[d];
	}
	return out;
}

memChunkIterator::coordinate_type memChunkIterator::coorIn2Out()
{
	return this->coorIn2Out(this->coor_);
}

memChunkIterator::coordinate_type memChunkIterator::ceP()
{
	coordinate_type ceP(this->dSize());
	for(dimensionId d = 0; d < this->dSize(); d++)
	{
		ceP[d] = this->csP_[d] + this->dims_[d];
	}

	return ceP;
}

memChunkIterator::coordinate_type memChunkIterator::outCoor()
{
	coordinate_type out(this->dSize());
	for(dimensionId d = 0; d < this->dSize(); d++)
	{
		out[d] = this->coor_[d] + this->csP_[d];
	}
	return out;
}

memChunkIterator::coordinate_type memChunkIterator::innerCoor()
{
	return this->coor_;
}

}
