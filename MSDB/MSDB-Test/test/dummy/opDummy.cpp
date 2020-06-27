#include "pch.h"
#include "opDummy.h"

namespace msdb
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_encode_plan>& plan,
					  std::shared_ptr<wavelet_encode_action>& action,
					  pQuery& qry)
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

void getWaveletDecode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_decode_plan>& plan,
					  std::shared_ptr<wavelet_decode_action>& action,
					  pQuery& qry)
{
	pQuery wdQuery_ = std::make_shared<query>();
	auto wdPlan_ = std::make_shared<wavelet_decode_plan>();
	auto wdAction_ = std::make_shared<wavelet_decode_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};
	auto pSet = std::make_shared<wavelet_encode_pset>(params);

	wdPlan_->setParamSet(pSet);
	wdAction_->setArrayDesc(wdPlan_->inferSchema());
	wdAction_->setParams(params);

	plan = wdPlan_;
	action = wdAction_;
	qry = wdQuery_;
}

void getMmtBuild(pArrayDesc sourceArrDesc, eleDefault level, std::shared_ptr<mmt_build_plan>& plan, std::shared_ptr<mmt_build_action>& action, pQuery& qry)
{
	pQuery weQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_build_plan>();
	auto mmtAction_ = std::make_shared<mmt_build_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};
	auto pSet = std::make_shared<wavelet_encode_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = weQuery_;
}
}	// caDummy
}	// msdb
