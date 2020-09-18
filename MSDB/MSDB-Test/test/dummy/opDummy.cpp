#include "pch.h"

void msdb::caDummy::tearDownQuery(pQuery qry)
{
	auto myTimer = qry->getTimer();
	myTimer->printTime();
}
