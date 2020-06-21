#include "pch.h"
#include "opDummy.h"

namespace msdb
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc sourceArrDesc, eleDefault level, std::shared_ptr<wavelet_encode_plan>& plan, std::shared_ptr<wavelet_encode_action>& action, pQuery& qry)
{
	pQuery weQuery_ = std::make_shared<query>();
	auto wePlan_ = std::make_shared<wavelet_encode_plan>();
	auto weAction_ = std::make_shared<wavelet_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};
	auto pSet = std::make_shared<wavelet_encode_pset>(params);

	wePlan_->setParamSet(pSet);
	weAction_->setArrayDesc(wePlan_->inferSchema());
	weAction_->setParams(params);

	plan = wePlan_;
	action = weAction_;
	qry = weQuery_;
}

}
}
