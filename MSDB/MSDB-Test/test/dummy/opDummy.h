#pragma once
#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>

#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_save/mmt_save_action.h>
#include <op/mmt_load/mmt_load_plan.h>
#include <op/mmt_load/mmt_load_action.h>

#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>
#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_compression_action.h>
#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>

namespace msdb
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_encode_plan>& plan,
					  std::shared_ptr<wavelet_encode_action>& action,
					  pQuery& qry);

void getWaveletDecode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_decode_plan>& plan,
					  std::shared_ptr<wavelet_decode_action>& action,
					  pQuery& qry);

void getMmtBuild(pArrayDesc sourceArrDesc, eleDefault& level,
				 std::shared_ptr<mmt_build_plan>& plan,
				 std::shared_ptr<mmt_build_action>& action,
				 pQuery& qry);

void getMmtSave(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_save_plan>& plan,
				std::shared_ptr<mmt_save_action>& action,
				pQuery& qry);

void getMmtLoad(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_load_plan>& plan,
				std::shared_ptr<mmt_load_action>& action,
				pQuery& qry);

void getMmtDeltaEncode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_encode_plan>& plan,
					   std::shared_ptr<mmt_delta_encode_action>& action,
					   pQuery& qry);

void getMmtDeltaDecode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_decode_plan>& plan,
					   std::shared_ptr<mmt_delta_decode_action>& action,
					   pQuery& qry);

void getSeCompression(pArrayDesc sourceArrDesc,
					  std::shared_ptr<se_compression_plan>& plan,
					  std::shared_ptr<se_compression_action>& action,
					  pQuery& qry);

void getSeDecompression(pArrayDesc sourceArrDesc, eleDefault level,
						std::shared_ptr<se_decompression_plan>& plan,
						std::shared_ptr<se_decompression_action>& action,
						pQuery& qry);

void getSPIHTEncode(pArrayDesc sourceArrDesc,
					std::shared_ptr<spiht_encode_plan>& plan,
					std::shared_ptr<spiht_encode_action>& action,
					pQuery& qry);

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getOperator(pArrayDesc sourceArrDesc, parameters& params)
{
	pQuery myQuery = std::make_shared<query>();
	auto myPlan = std::make_shared<plan_>();
	auto myAction = std::make_shared<action_>();

	myPlan->setParamSet(std::make_shared<pset_>(params));
	myAction->setArrayDesc(myPlan->inferSchema());
	myAction->setParams(params);

	return std::make_tuple(myPlan, myAction, myQuery);
}

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getSingleArrayParamOperator(pArrayDesc sourceArrDesc)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};

	return getOperator<plan_, action_, pset_>(sourceArrDesc, params);
}

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getLevelArrayParamOperator(pArrayDesc sourceArrDesc, eleDefault& level)
{
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc),
		std::make_shared<opParamConst>(std::make_shared<stableElement>(&level, _ELE_DEFAULT_TYPE))
	};

	return getOperator<plan_, action_, pset_>(sourceArrDesc, params);
}
}
}