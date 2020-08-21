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
pBitmap opPlan::inferBitmap()
{
	return this->myParamSet_->inferBitmap();
}
parameters opPlan::getParam()
{
	return this->myParamSet_->getParam();
}
opParamPlan::opParamPlan(pPlan plan)
	: plan_(plan)
{
}
opParam::void_pointer opParamPlan::getParam()
{
	return this->plan_;
}
opParamType opParamPlan::type()
{
	return opParamType::PLAN;
}
opParamPlanPlaceholder::opParamPlanPlaceholder()
	: opParamPlaceholder(), opParamPlan(nullptr)
{
}
opParamType opParamPlanPlaceholder::type()
{
	return opParamType::PLAN;
}
//void opPlan::addParamSet(pParamSet pSet)
//{
//	this->paramSets_.push_back(pSet);
//}
}
