#include <stdafx.h>
#include <query/opPlan.h>
#include <util/logger.h>

namespace msdb
{
opPlan::opPlan()
	: parentPlan_(nullptr), outArrBitmap_(nullptr), inParamSet_(nullptr)
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
pBitmapTree opPlan::inferBitmap()
{
	if (this->outArrBitmap_)
		return this->outArrBitmap_;

	this->outArrBitmap_ = this->inferBottomUpBitmap();
	this->outArrBitmap_ = this->inferTopDownBitmap();
	return outArrBitmap_;
}
pBitmapTree opPlan::inferBottomUpBitmap()
{
	return this->inParamSet_->inferBottomUpBitmap();
}
pBitmapTree opPlan::inferTopDownBitmap()
{
	if(this->parentPlan_)
	{
		return this->inParamSet_->inferTopDownBitmap(
			this->parentPlan_->inferBitmap());
	}else
	{
		assert(this->outArrBitmap_ != nullptr);
		return this->inParamSet_->inferTopDownBitmap(
			this->outArrBitmap_);
	}
}
pAction opPlan::getAction()
{
	BOOST_LOG_TRIVIAL(debug) << "getAction: " << this->name();
	auto myAction = this->makeAction();
	myAction->setParams(this->getParam());
	myAction->setArrayDesc(this->inferSchema());
	myAction->setPlanBitmap(this->inferBitmap());
	return myAction;
}
parameters opPlan::getParam()
{
	return this->inParamSet_->getParam();
}
void opPlan::setParentPlan(pPlan parentPlan)
{
	this->parentPlan_ = parentPlan;
}
}
