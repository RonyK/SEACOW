#include "pch.h"

void msdb::caDummy::dearDownQuery(pQuery qry)
{
	auto myTimer = qry->getTimer();
	myTimer->printTime();
}
