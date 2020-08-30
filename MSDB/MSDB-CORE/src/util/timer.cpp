#include <util/timer.h>

namespace msdb
{
timer::timer()
	: jobId_(0)
{
}

void timer::start(size_t threadId)
{
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
}

void timer::check(size_t threadId, workType sType, bool curJobEnd)
{
	size_t curJobId = this->curJobIds_[threadId];
	
	std::chrono::duration<double> record = std::chrono::system_clock::now() - this->curJobTimes_[threadId];
	this->records_.push_back({threadId, record, curJobId, sType});
	if (curJobEnd)
	{
		this->curJobIds_[threadId] = this->getNextJobId();
	}
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
}

void timer::printTime()
{
	for (int i = 0; i < this->records_.size(); i++)
	{
		std::cout << this->records_[i].threadId << "\t";
		std::cout << this->records_[i].jobId << "\t";
		std::cout << this->strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "\t";
		std::cout << this->records_[i].time_.count() << std::endl;
	}
}
size_t timer::getNextJobId()
{
	this->mutexJobId_.lock();
	size_t outJobId = this->jobId_++;
	this->mutexJobId_.unlock();

	return outJobId;
}
}