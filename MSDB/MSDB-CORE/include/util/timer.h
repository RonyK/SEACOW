#pragma once
#ifndef _MSDB_TIMER_H_
#define _MSDB_TIMER_H_

#include <chrono>
#include <iostream>
#include <vector>
#include <mutex>
#include <map>

namespace msdb
{
class timer
{
public:
	//enum class bigType { scan=0, between };
	//std::vector<const char*> bigString = { "scan", "between" };
	enum class workType 
	{ 
		IDLE = 0,
		IO,
		COMPUTING,
		LOGGING,
		ARRAY_CONSTRUCTING,
		OTHERS,
		TIMER_STOP
	};
	std::vector<const char*> strTimerWorkType = { "IDLE", "IO", "COMPUTING", "LOGGING", "ARRAY_CONSTRUCTING", "OTHERS", "TIMER_STOP" };

public:
	timer();

public:
	void start(size_t threadId);
	void check(size_t threadId, workType sType, bool curJobEnd = false);
	void printTime();
	size_t getNextJobId();
	// TODO:: Job, Job name, getJobId

private:
	typedef struct checkPoint
	{
		size_t threadId;
		std::chrono::duration<double> time_;
		size_t jobId;
		workType stype_;
	}checkPoint;

private:
	std::chrono::system_clock::time_point time_;
	std::vector<checkPoint> records_;
	std::mutex mutexJobId_;
	size_t jobId_;
	std::map<size_t, size_t> curJobIds_;
	std::map<size_t, std::chrono::system_clock::time_point> curJobTimes_;
};
}

#endif // !_MSDB_TIMER_H_
