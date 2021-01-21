#include <stdafx.h>
#include <util/boostUtil.h>

namespace msdb
{
unsigned long getThreadId()
{
	//std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
	//unsigned long threadNumber = 0;
	//scanf(threadId.c_str(), "%lx", &threadNumber);
	//return threadNumber;

	std::ostringstream ss;
	ss << std::this_thread::get_id();
	std::string sThrId = ss.str();
	
	return atol(ss.str().c_str());
}
}
