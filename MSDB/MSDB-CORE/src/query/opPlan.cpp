#include <query/opPlan.h>

namespace msdb
{
opPlan::opPlan()
{
	this->initParamSets();
}
pArrayDesc opPlan::inferSchema(pParamSet oppSet)
{
	return oppSet->inferSchema();
}
void opPlan::addParamSet(pParamSet pSet)
{
	this->paramSets_.push_back(pSet);
}
}
