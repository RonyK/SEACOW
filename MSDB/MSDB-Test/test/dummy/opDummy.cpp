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

void getMmtBuild(pArrayDesc sourceArrDesc,
				 eleDefault level,
				 std::shared_ptr<mmt_build_plan>& plan,
				 std::shared_ptr<mmt_build_action>& action,
				 pQuery& qry)
{
	pQuery mmtQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_build_plan>();
	auto mmtAction_ = std::make_shared<mmt_build_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};
	auto pSet = std::make_shared<mmt_build_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = mmtQuery_;
}
void getMmtSave(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_save_plan>& plan,
				std::shared_ptr<mmt_save_action>& action,
				pQuery& qry)
{
	pQuery mmtQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_save_plan>();
	auto mmtAction_ = std::make_shared<mmt_save_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<mmt_save_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = mmtQuery_;
}
void getMmtLoad(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_load_plan>& plan,
				std::shared_ptr<mmt_load_action>& action,
				pQuery& qry)
{
	pQuery mmtQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_load_plan>();
	auto mmtAction_ = std::make_shared<mmt_load_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<mmt_load_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = mmtQuery_;
}

void getMmtDeltaEncode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_encode_plan>& plan,
					   std::shared_ptr<mmt_delta_encode_action>& action,
					   pQuery& qry)
{
	pQuery mmtQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_delta_encode_plan>();
	auto mmtAction_ = std::make_shared<mmt_delta_encode_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<mmt_delta_encode_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = mmtQuery_;
}

void getMmtDeltaDecode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_decode_plan>& plan,
					   std::shared_ptr<mmt_delta_decode_action>& action,
					   pQuery& qry)
{
	pQuery mmtQuery_ = std::make_shared<query>();
	auto mmtPlan_ = std::make_shared<mmt_delta_decode_plan>();
	auto mmtAction_ = std::make_shared<mmt_delta_decode_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<mmt_delta_decode_pset>(params);

	mmtPlan_->setParamSet(pSet);
	mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
	mmtAction_->setParams(params);

	plan = mmtPlan_;
	action = mmtAction_;
	qry = mmtQuery_;
}

void getSeCompression(pArrayDesc sourceArrDesc,
					  std::shared_ptr<se_compression_plan>& plan,
					  std::shared_ptr<se_compression_action>& action,
					  pQuery& qry)
{
	pQuery seQuery_ = std::make_shared<query>();
	auto sePlan_ = std::make_shared<se_compression_plan>();
	auto seAction_ = std::make_shared<se_compression_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<se_compression_pset>(params);

	sePlan_->setParamSet(pSet);
	seAction_->setArrayDesc(sePlan_->inferSchema());
	seAction_->setParams(params);

	plan = sePlan_;
	action = seAction_;
	qry = seQuery_;
}
}	// caDummy
}	// msdb
