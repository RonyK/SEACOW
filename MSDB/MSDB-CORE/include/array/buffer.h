#pragma once
#ifndef _MSDB_BUFFER_H_
#define _MSDB_BUFFER_H_

namespace msdb
{
class buffer
{
	virtual void* getData() = 0;

	virtual void const* getReadData() const = 0;

	virtual size_t getSize() const = 0;

	virtual void free();

	virtual bool pin() const = 0;
	virtual void unPin() const = 0;
};
}

#endif	// _MSDB_BUFFER_H_
