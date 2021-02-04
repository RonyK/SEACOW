#pragma once
#ifndef _MSDB_IOUTIL_H_
#define _MSDB_IOUTIL_H_

#include <stdafx.h>

namespace msdb
{
inline size_t getSize(std::stringstream& ss)
{
	ss.seekg(0, std::ios::end);
	size_t ssSize = ss.tellg();
	ss.seekg(0, std::ios::beg);

	return ssSize;
}
}		// msdb
#endif	// _MSDB_IOUTIL_H_