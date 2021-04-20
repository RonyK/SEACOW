#pragma once
#ifndef _MSDB_EXPERIMENT_RECORDER
#define _MSDB_EXPERIMENT_RECORDER

#include <stdafx.h>
#include <util/singleton.h>

namespace msdb
{
class experimentRecorder;
using pRecorder = std::shared_ptr<experimentRecorder>;

class experimentRecorder : public singleton<experimentRecorder>
{
public:
	experimentRecorder();
	~experimentRecorder();

public:
	void insert(size_t experimentId, size_t trialId, size_t dataId, size_t jobId, std::string jobName, float procTime);
	void print();

private:
	typedef struct record
	{
		size_t experimentId;
		size_t trialId;
		size_t dataId;
		size_t opId;
		std::string opName;
		float procTime;
	};

private:
	std::vector<record> records_;
	std::set<size_t> experimentIds_;
	std::set<size_t> dataIds_;
};

}

#endif	// _MSDB_EXPERIMENT_RECORDER