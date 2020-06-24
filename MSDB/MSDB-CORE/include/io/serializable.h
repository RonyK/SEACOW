#pragma once
#ifndef _MSDB_SERIALIZABLE_H_
#define _MSDB_SERIALIZABLE_H_

#include <iostream>

namespace msdb
{
using header_version_t = size_t;
using header_size_t = size_t;

class serialHeader;
using pHeader = std::shared_ptr<serialHeader>;

class serializable;
using pSerializable = std::shared_ptr<serializable>;

class serialHeader
{
public:
	serialHeader();

public:
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

public:
	header_version_t version_;
	header_size_t size_;
	//header_size_t headerSize_;	// Not used
};	// serialHeader

class serializable
{
public:
	serializable(pHeader myHeader);

public:
	pHeader getHeader();		// header without any update
	pHeader getInHeader();		// header used to deserialize
	pHeader getOutHeader();		// header used to serialize
	
	virtual void updateToHeader() = 0;
	virtual void updateFromHeader() = 0;
	
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

private:
	pHeader myHeader_;
};	// serializable
}

#endif	// _MSDB_SERIALIZABLE_H_
