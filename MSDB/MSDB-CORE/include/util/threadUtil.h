#pragma once
#ifndef _MSDB_THREADUTIL_
#define _MSDB_THREADUTIL_

#define _MSDB_ACTION_THREAD_NUM_	8

#include <stdafx.h>

namespace msdb
{
inline unsigned long getThreadId()
{
	return atol(boost::lexical_cast<std::string>(boost::this_thread::get_id()).c_str());
}
}
#endif // _MSDB_THREADUTIL_


