#include <query/opPlan.h>

namespace msdb
{
opPlan::opPlan()
{
}
void opPlan::setParamSet(pParamSet paramSet)
{
	this->inParamSet_ = paramSet;

	auto params = this->getParam();
	for(auto param : params)
	{
		if(param->type() == opParamType::PLAN)
		{
			auto childPlan = std::static_pointer_cast<opParamPlan::paramType>(param->getParam());
			childPlan->setParentPlan(shared_from_this());
		}
	}
}
pArrayDesc opPlan::inferSchema()
{
	return this->inParamSet_->inferSchema();
}
pBitmap opPlan::inferBitmap()
{
	if (this->outArrBitmap_)
		return this->outArrBitmap_;

	this->outArrBitmap_ = this->inferBottomUpBitmap();
	this->outArrBitmap_ = this->inferTopDownBitmap();
	return outArrBitmap_;
}
pBitmap opPlan::inferBottomUpBitmap()
{
	return this->inParamSet_->inferBottomUpBitmap();
}
pBitmap opPlan::inferTopDownBitmap()
{
	//return pBitmap();
	return nullptr;
}
parameters opPlan::getParam()
{
	return this->inParamSet_->getParam();
}
void opPlan::setParentPlan(pPlan parentPlan)
{
	this->parentPlan_ = parentPlan;
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
opPlanParamSet::opPlanParamSet(parameters& pSet)
{
	assert(this->params_[0]->type() == opParamType::PLAN);		// source plan
}
pArrayDesc opPlanParamSet::inferSchema()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	return std::make_shared<arrayDesc>(*sourcePlan->inferSchema());
}
pBitmap opPlanParamSet::inferBottomUpBitmap()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	//return std::make_shared<bitmap>(*sourcePlan->inferBottomUpBitmap());
	return nullptr;
}
pBitmap opPlanParamSet::inferTopDownBitmap(pBitmap fromParent)
{
	return std::make_shared<bitmap>(*fromParent);
}
}
