#pragma once
#ifndef _MSDB_TIMER_H_
#define _MSDB_TIMER_H_

#include <chrono>
#include <iostream>
#include <vector>

namespace msdb
{
class timer
{
public:
	enum class bigType { scan=0, between };
	std::vector<const char*> bigString = { "scan", "between" };
	enum class smallType { IO=0, compute, print };
	std::vector<const char*> smallString = { "IO", "compute", "print" };

public:
	timer();
	void start();
	void check(int id, bigType bType, smallType sType);
	void printTime();

private:
	typedef struct checkPoint
	{
		int threadId;
		std::chrono::duration<double> time_;
		bigType btype_;
		smallType stype_;
	}checkPoint;

private:
	std::chrono::system_clock::time_point time_;
	std::vector<checkPoint> records_;
};
}

#endif // !_MSDB_TIMER_H_
