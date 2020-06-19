#include <array/chunkBuffer.h>
#include <system/exceptions.h>
#include <iostream>
#include <algorithm>

namespace msdb
{
chunkBuffer::chunkBuffer()
	: size_(0), isAllocated_(false), data_(nullptr)
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
	return this->size_;
}

void chunkBuffer::alloc(bufferSize size)
{
	assert(size > 0);

	if(this->isAllocated())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}
	
	this->data_ = new char[size];
	this->size_ = size;
	this->isAllocated_ = true;
}

void chunkBuffer::realloc(bufferSize size)
{
	assert(size > 0);

	if(!this->isAllocated())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_REALLOC_FAIL));
	}

	void* re = new char[size];
	memcpy(re, this->data_, std::min(size, this->size_));
	this->free();
	
	this->data_ = re;
	this->size_ = size;
}

void chunkBuffer::copy(void* data, bufferSize size)
{
	this->free();
	this->data_ = new char[size];
	memcpy(this->data_, data, size);
	this->size_ = size;
}

// WARNING:: data is deleted when the chunkBuffer is disappear in a destructor.
void chunkBuffer::assign(void* data, bufferSize size)
{
	this->free();
	this->data_ = data;
	this->size_ = size;
}

void chunkBuffer::free()
{
	if(this->data_ != nullptr && this->size_ != 0)
	{
		delete[] this->data_;
		this->size_ = 0;
		this->data_ = nullptr;
	}
}
bool chunkBuffer::isAllocated()
{
	if (this->isAllocated_ && this->data_ != nullptr)
		return true;
	return false;
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
