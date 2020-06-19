#include <query/opPlan.h>

namespace msdb
{
opPlan::opPlan()
{
	//this->initParamSets();
}
void opPlan::setParamSet(pParamSet paramSet)
{
	this->myParamSet_ = paramSet;
}
pArrayDesc opPlan::inferSchema()
{
	return this->myParamSet_->inferSchema();
}
parameters opPlan::getParam()
{
	return this->myParamSet_->getParam();
}
//void opPlan::addParamSet(pParamSet pSet)
//{
//	this->paramSets_.push_back(pSet);
//}
}
