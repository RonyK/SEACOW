#include <query/opPlan.h>

namespace msdb
{
opPlan::opPlan()
{
	this->initParamSets();
}
arrayDesc opPlan::inferSchema(paramSet oppSet)
{
	return oppSet->inferSchema();
}
void opPlan::addParamSet(paramSet pSet)
{
	this->paramSets_.push_back(pSet);
}
}
