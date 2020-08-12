#include <op/spiht_encode/timer.h>

namespace msdb
{
timer::timer()
{
}

void timer::start()
{
	this->time_ = std::chrono::system_clock::now();
}

void timer::check(int id, bigType bType, smallType sType)
{
	std::chrono::duration<double> record = std::chrono::system_clock::now() - this->time_;
	this->records_.push_back({id, record, bType, sType});
	this->time_ = std::chrono::system_clock::now();
}

void timer::printTime()
{
	for (int i = 0; i < this->records_.size(); i++)
	{
		std::cout << this->records_[i].threadId << "\t";
		std::cout << this->bigString[static_cast<int>(this->records_[i].btype_)] << "\t";
		std::cout << this->smallString[static_cast<int>(this->records_[i].stype_)] << "\t";
		std::cout << this->records_[i].time_.count() << std::endl;
	}
}
}