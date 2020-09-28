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
	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->curJobType_[threadId] = nextWorkType;
	this->jobName_[curJobId] = nextJobName;
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

	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->jobName_[curJobId] = nextJobName;
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
	std::map<size_t, float> thread;
	std::map<std::string, float> job;
	std::map<std::string, float> workType;

	std::map<std::string, float> jobWork;

	for (int i = 0; i < this->records_.size(); i++)
	{
		BOOST_LOG_TRIVIAL(debug) << 
			this->records_[i].threadId << "\t" << 
			this->records_[i].jobId << "\t" << 
			boost::str(boost::format("%f") % this->records_[i].time_.count()) << "\t" << 
			"[" << jobName_[this->records_[i].jobId] << " / " << 
			strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "]";

		if (thread.find(this->records_[i].threadId) != thread.end())
		{
			thread.find(this->records_[i].threadId)->second += this->records_[i].time_.count();
		}
		else
		{
			thread.insert(std::make_pair(this->records_[i].threadId, this->records_[i].time_.count()));
		}

		if (job.find(jobName_[this->records_[i].jobId]) != job.end())
		{
			job.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
		}
		else
		{
			job.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
		}

		if (workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != workType.end())
		{
			workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		}
		else
		{
			workType.insert(std::make_pair(strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}

		if (jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != jobWork.end())
		{
			jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		}
		else
		{
			jobWork.insert(std::make_pair(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}
	}

	BOOST_LOG_TRIVIAL(debug) << '\n';
	BOOST_LOG_TRIVIAL(debug) << "=====threadId=====";
	for (auto it = thread.begin(); it != thread.end(); it++) {
		BOOST_LOG_TRIVIAL(debug) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(debug) << '\n';
	BOOST_LOG_TRIVIAL(debug) << "=====jobName=====";
	for (auto it = job.begin(); it != job.end(); it++) {
		BOOST_LOG_TRIVIAL(debug) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(debug) << '\n';
	BOOST_LOG_TRIVIAL(debug) << "=====WorkType=====";
	for (auto it = workType.begin(); it != workType.end(); it++) {
		BOOST_LOG_TRIVIAL(debug) <<
			it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(debug) << '\n';
	BOOST_LOG_TRIVIAL(debug) << "=====jobName, WorkType=====";
	for (auto it = jobWork.begin(); it != jobWork.end(); it++) {
		BOOST_LOG_TRIVIAL(debug) <<
			it->second << " [" <<
			it->first << "]";
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