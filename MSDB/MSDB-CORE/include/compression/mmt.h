#pragma once
#ifndef _MSDB_MMT_H_
#define _MSDB_MMT_H_

#include <io/bitstream.h>
#include <memory>

namespace msdb
{
class mmt;
using pMMT = std::shared_ptr<mmt>;

class mmt
{

};

class mmtHolder
{
public:
	
	mmtHolder(pMMT inMMT);

protected:
	pMMT mmt_;
};

class inMmtHolder : public mmtHolder
{

};

class outMmtHolder : public mmtHolder
{

};

class serializedMMTHolder
{
public:
	serializedMMTHolder(pBstream bs);
	serializedMMTHolder(void* data, size_t length);

protected:
	pBstream bs_;
};
}

#endif		// _MSDB_MMT_H_