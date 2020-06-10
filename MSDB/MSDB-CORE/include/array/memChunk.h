#pragma once
#ifndef _MSDB_RAWCHUNK_H_
#define _MSDB_RAWCHUNK_H_

#include <array/chunkBuffer.h>
#include <util/coordinate.h>

namespace msdb
{
class memChunk : public chunkBuffer
{
public:
	memChunk();

public:
	virtual bool isCompressed() const;
};

//class rawChunkIterator : public coorItr
//{
//public:
//	rawChunkIterator(value_t* data, const size_t dSize, position_t* boundary);
//
//public:
//	virtual void moveTo(const coordinate<coorItr::dim_type>& coor);
//	coordinate<coorItr::dim_type> coor();
//
//private:
//	coorItr::dim_pointer csP_;		// chunk start point
//	coorItr::dim_pointer ceP_;		// chunk end point
//};
}

#endif		// _MSDB_RAWCHUNK_H_