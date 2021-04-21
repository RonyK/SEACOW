#include "pch.h"

void msdb::caDummy::tearDownQuery(pQuery qry, size_t expId, size_t expTrial, size_t dataId, size_t methodId)
{
	auto myTimer = qry->getTimer();
	myTimer->printTime();
	myTimer->recordTime(expId, expTrial, dataId, methodId);
}
