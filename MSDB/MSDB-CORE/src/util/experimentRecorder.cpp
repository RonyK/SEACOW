#include <stdafx.h>
#include <util/experimentRecorder.h>
#include <util/logger.h>

namespace msdb
{
experimentRecorder::experimentRecorder()
{

}

experimentRecorder::~experimentRecorder()
{
	this->print();
}

void experimentRecorder::insert(size_t experimentId, size_t trialId, size_t dataId, size_t opId, std::string opName, float procTime, size_t methodId)
{
	this->experimentIds_.insert(experimentId);
	this->dataIds_.insert(dataId);

	this->records_.push_back(record{ experimentId, trialId, dataId, opId, opName, procTime, methodId });
}

void experimentRecorder::print()
{
	auto expIdIt = this->experimentIds_.begin();
	while(expIdIt != this->experimentIds_.end())
	{
		BOOST_LOG_TRIVIAL(info) << "##############################";
		BOOST_LOG_TRIVIAL(info) << "EXPERIMENT";
		BOOST_LOG_TRIVIAL(info) << "##############################";

		// Collect Same Experiment data / trial
		size_t curExpId = *expIdIt;
		std::vector<std::vector<record>> curExpRecords;

		for(auto r : this->records_)
		{
			if(r.experimentId == curExpId)
			{
				assert(r.trialId < 10);

				while(curExpRecords.size() <= r.trialId)
				{
					curExpRecords.push_back(std::vector<record>());
				}

				curExpRecords[r.trialId].push_back(r);
			}
		}

		// Same Exp, Same Trial
		int curTrial = 0;
		for(auto curTrialRecords : curExpRecords)
		{
			BOOST_LOG_TRIVIAL(info) << "------------------------------";
			BOOST_LOG_TRIVIAL(info) << "CUR TRIAL: " << curTrial;
			BOOST_LOG_TRIVIAL(info) << "------------------------------";

			// Collect Same Data
			std::map<size_t, std::vector<std::vector<record>>> trialRecords;

			auto dataIdIt = this->dataIds_.begin();
			while(dataIdIt != this->dataIds_.end())
			{
				for(auto r : curTrialRecords)
				{
					if(r.dataId == *dataIdIt)
					{
						if (trialRecords.find(r.dataId) == trialRecords.end())
						{
							trialRecords[r.dataId] = std::vector<std::vector<record>>();
						}

						while(trialRecords[r.dataId].size() <= r.methodId)
						{
							trialRecords[r.dataId].push_back(std::vector<record>());
						}

						trialRecords[r.dataId][r.methodId].push_back(r);
					}
				}

				++dataIdIt;
			}

			for(auto dataRecords : trialRecords)
			{
				size_t methodId = 0;
				for(auto methodRecords : dataRecords.second)
				{
					std::map<size_t, record> line;
					for (auto r : methodRecords)
					{
						line[r.opId] = r;
					}

					std::stringstream ss;
					ss << "{" << dataRecords.first << "-" << methodId << "} / ";	// dataId
					for (auto r : line)
					{
						ss << boost::format("%1$.5f") % r.second.procTime << ", ";		// procTimes
					}

					ss << " / ";
					for (auto r : line)
					{
						ss << "[" << r.second.opName << "] ";	// operator Names
					}

					BOOST_LOG_TRIVIAL(info) << ss.str();		// print
					++methodId;
				}
			}
			++curTrial;
		}
		
		BOOST_LOG_TRIVIAL(info) << "#";		// print new line

		++expIdIt;
	}
}
}	// msdb