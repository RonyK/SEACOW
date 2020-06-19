#pragma once
#ifndef _MSDB_BUFFER_H_
#define _MSDB_BUFFER_H_

namespace msdb
{
using bufferSize = unsigned long long;
extern const bufferSize INVALID_BUFFER_SIZE;

// abstrct class for binary data holding
class buffer
{
public:
	buffer();

public:
	virtual void* getData() = 0;								// writable data pointer
	virtual void const* getReadData() const = 0;				// read-only data pointer

	virtual bufferSize size() const = 0;						// buffer size

	virtual void alloc(bufferSize size) = 0;
	virtual void realloc(bufferSize size) = 0;
	virtual void copy(void* data, bufferSize size) = 0;			// copy input data
	virtual void free();										// free binary data

	//virtual bool pin() const = 0;
	//virtual void unPin() const = 0;
};
}

#endif	// _MSDB_BUFFER_H_
