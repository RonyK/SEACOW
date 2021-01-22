#include <stdafx.h>
#include <util/timer.h>
#include <util/logger.h>

namespace msdb
{
extern std::vector<const char*> strTimerWorkType = {
	"IDLE", "IO", "COMPUTING", "LOGGING", "ARRAY_CONSTRUCTING", "OTHERS", "TIMER_STOP"
};

timer::timer()
	: jobId_(0)
{
	static bool isBoostInit = initBoostLogger();
}

void timer::start(size_t threadId, const std::string& nextJobName, workType nextWorkType)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	this->_start_NoLock_(threadId, nextJobName, nextWorkType);
	//////////////////////////////
}

void timer::nextWork(size_t threadId, workType nextWorkType)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	this->_nextWork_NoLock_(threadId, nextWorkType);
	//////////////////////////////
}

void timer::nextWork(size_t threadId, size_t parentThreadId, workType nextWorkType)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if(curJobIds_.find(threadId) == curJobIds_.end())
	{
		this->curJobIds_[threadId] = this->curJobIds_[parentThreadId];
		this->curJobType_[threadId] = nextWorkType;
	}

	this->_nextWork_NoLock_(threadId, nextWorkType);
	//////////////////////////////
}

void timer::_start_NoLock_(size_t threadId, const std::string& nextJobName, workType nextWorkType)
{
	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->curJobType_[threadId] = nextWorkType;
	this->jobName_[curJobId] = nextJobName;
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
}

void timer::_nextWork_NoLock_(size_t threadId, workType nextWorkType)
{
	std::chrono::duration<double> record = std::chrono::system_clock::now() - this->curJobTimes_[threadId];
	this->records_.push_back({ threadId, record, this->curJobIds_[threadId], this->curJobType_[threadId] });
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
	this->curJobType_[threadId] = nextWorkType;
}

void timer::nextJob(size_t threadId, const std::string& nextJobName, workType nextWorkType)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if (curJobIds_.find(threadId) == curJobIds_.end())
	{
		this->_start_NoLock_(threadId, nextJobName, nextWorkType);
	}else
	{
		this->_nextWork_NoLock_(threadId, nextWorkType);
	}

	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->jobName_[curJobId] = nextJobName;
	//////////////////////////////
}

void timer::pause(size_t threadId)
{
	this->nextJob(threadId, "Pause", workType::TIMER_STOP);
}

void timer::resume(size_t threadId, const std::string& nextJobName, workType nextJobType)
{
	this->nextJob(threadId, nextJobName, nextJobType);
}

void timer::printTime(bool printDetail)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	std::map<size_t, float> thread;
	std::map<std::string, float> job;
	std::map<std::string, float> workType;

	std::map<std::string, float> jobWork;

	for (int i = 0; i < this->records_.size(); i++)
	{
		if(printDetail)
		{
			BOOST_LOG_TRIVIAL(info) <<
				this->records_[i].threadId << "\t" <<
				this->records_[i].jobId << "\t" <<
				boost::str(boost::format("%f") % this->records_[i].time_.count()) << "\t" <<
				"[" << jobName_[this->records_[i].jobId] << " / " <<
				strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "]";
		}
		
		if (thread.find(this->records_[i].threadId) != thread.end())
		{
			thread.find(this->records_[i].threadId)->second += this->records_[i].time_.count();
		} else
		{
			thread.insert(std::make_pair(this->records_[i].threadId, this->records_[i].time_.count()));
		}

		if (job.find(jobName_[this->records_[i].jobId]) != job.end())
		{
			job.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
		} else
		{
			job.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
		}

		if (workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != workType.end())
		{
			workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		} else
		{
			workType.insert(std::make_pair(strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}

		if (jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != jobWork.end())
		{
			jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		} else
		{
			jobWork.insert(std::make_pair(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====threadId=====";
	for (auto it = thread.begin(); it != thread.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====jobName=====";
	for (auto it = job.begin(); it != job.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====WorkType=====";
	for (auto it = workType.begin(); it != workType.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====jobName, WorkType=====";
	for (auto it = jobWork.begin(); it != jobWork.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			it->second << " [" <<
			it->first << "]";
	}
	//////////////////////////////
}

size_t timer::getNextJobId()
{
	std::lock_guard<std::mutex> guard(this->mutexJobId_);

	size_t outJobId = this->jobId_++;

	return outJobId;
}
size_t timer::getMyJobId(size_t threadId)
{
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if(curJobIds_.find(threadId) != curJobIds_.end())
	{
		return this->curJobIds_[threadId];
	}

	return 0;
}

std::mutex timer::mutexJobId_;
std::mutex timer::mutexJobUpdate_;

}