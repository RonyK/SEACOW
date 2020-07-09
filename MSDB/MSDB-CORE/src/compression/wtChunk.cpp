#include <compression/wtChunk.h>

namespace msdb
{
wtChunk::wtChunk(pChunkDesc desc)
	: chunk(desc)
{
}

wtChunk::~wtChunk()
{
}

size_t wtChunk::getLevel()
{
	return this->level_;
}
chunkId wtChunk::getBandId()
{
	return this->bandId_;
}
chunkId wtChunk::getSourceChunkId()
{
	return this->sourceChunkId;
}

void wtChunk::setLevel(size_t level)
{
	this->level_ = level;
}
void wtChunk::setBandId(chunkId bid)
{
	this->bandId_ = bid;
}
void wtChunk::setSourceChunkId(chunkId cid)
{
	this->sourceChunkId = cid;
}
}	// msdb