#pragma once
#ifndef _MSDB_VALUE_DISTRIBUTION_RECODER_H_
#define _MSDB_VALUE_DISTRIBUTION_RECODER_H_

#include <stdafx.h>
#include <util/singleton.h>

namespace msdb
{
class valueDistributionRecorder : public singleton<valueDistributionRecorder>
{
private:
	valueDistributionRecorder();
	~valueDistributionRecorder();

public:
	void increase(size_t bits, uint8_t value);
	void printTable();

private:
	std::vector<std::vector<size_t>> table_;

protected:
	friend singleton<valueDistributionRecorder>;
};
}
#endif	// _MSDB_VALUE_DISTRIBUTION_RECODER_H_