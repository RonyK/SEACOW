#include <util/timer.h>

namespace msdb
{

extern std::vector<const char*> strTimerWorkType = {
	"IDLE", "IO", "COMPUTING", "LOGGING", "ARRAY_CONSTRUCTING", "OTHERS", "TIMER_STOP"
};

timer::timer()
	: jobId_(0)
{
}

void timer::start(size_t threadId, const std::string& nextJobName, workType nextWorkType)
{
	this->curJobIds_[threadId] = this->getNextJobId();
	this->curJobType_[threadId] = nextWorkType;
	this->curJobName_[threadId] = nextJobName;
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
}

void timer::nextWork(size_t threadId, workType nextWorkType)
{
	std::chrono::duration<double> record = std::chrono::system_clock::now() - this->curJobTimes_[threadId];
	this->records_.push_back({ threadId, record, this->curJobIds_[threadId], this->curJobType_[threadId]});
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
	this->curJobType_[threadId] = nextWorkType;
}

void timer::nextJob(size_t threadId, const std::string& nextJobName, workType nextWorkType)
{
	if (curJobIds_.find(threadId) == curJobIds_.end())
	{
		this->start(threadId, nextJobName, nextWorkType);
	}else
	{
		this->nextWork(threadId, nextWorkType);
	}
	this->curJobIds_[threadId] = this->getNextJobId();
	this->curJobName_[threadId] = nextJobName;
}

void timer::pause(size_t threadId)
{
	this->nextJob(threadId, "Pause", workType::TIMER_STOP);
}

void timer::resume(size_t threadId, const std::string& nextJobName, workType nextJobType)
{
	this->nextJob(threadId, nextJobName, nextJobType);
}

void timer::printTime()
{
	for (int i = 0; i < this->records_.size(); i++)
	{
		std::cout << this->records_[i].threadId << "\t";
		std::cout << this->records_[i].jobId << "\t";
		std::cout << strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "\t";
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