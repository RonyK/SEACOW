#include <array/chunkBuffer.h>
#include <system/exceptions.h>
#include <iostream>
#include <algorithm>

namespace msdb
{
chunkBuffer::chunkBuffer()
	: buffer(), data_(nullptr)
{

}

chunkBuffer::~chunkBuffer()
{
	this->free();
}

void* chunkBuffer::getData()
{
	return this->data_;
}

void const* chunkBuffer::getReadData() const
{
	return this->data_;
}

bufferSize msdb::chunkBuffer::size() const
{
	return this->bodySize_;
}

void chunkBuffer::alloc(bufferSize size)
{
	assert(size > 0);

	if(this->isAllocated())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}
	
	this->isAllocated_ = true;
	this->data_ = new char[size];
	this->bodySize_ = size;
	this->isAllocated_ = true;
}

void chunkBuffer::realloc(bufferSize size)
{
	assert(size > 0);

	void* re = new char[size];
	memcpy(re, this->data_, std::min(size, this->bodySize_));
	this->free();

	this->isAllocated_ = true;
	this->data_ = re;
	this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, bufferSize size)
{
	assert(size <= this->bodySize_);
	//this->free();
	//this->isAllocated_ = true;
	//this->data_ = new char[size];
	memcpy(this->data_, data, size);
	//this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, bufferSize offset, bufferSize size)
{
	assert(offset + size <= this->bodySize_);
	memcpy((char*)this->data_ + offset, data, size);

}

// WARNING:: data is deleted when the chunkBuffer is disappear in a destructor.
void chunkBuffer::assign(void* data, bufferSize size)
{
	this->free();
	this->isAllocated_ = false;
	this->data_ = data;
	this->bodySize_ = size;
}

void chunkBuffer::free()
{
	if(this->isAllocated_ && this->data_ != nullptr)
	{
		delete[] this->data_;
		this->isAllocated_ = false;
		this->bodySize_ = 0;
		this->data_ = nullptr;
	}
}

//void chunkBuffer::compress(CompressionMethod cm)
//{
//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_CHUNK_CANNOT_COMPRESS));
//}
//
//void chunkBuffer::uncompress()
//{
//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_CHUNK_CANNOT_UNCOMPRESS));
//}
}
