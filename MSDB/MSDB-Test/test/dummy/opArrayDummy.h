#pragma once
#ifndef _MSDB_OPARRAYDUMMY_H_
#define _MSDB_OPARRAYDUMMY_H_

#include <pch.h>

#include <op/save/save_action.h>
#include <op/save/save_plan.h>
#include <op/load/load_action.h>
#include <op/load/load_plan.h>

#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/spiht_decode/spiht_decode_action.h>

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

#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>
#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

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

void getSPIHTEncode(pArrayDesc sourceArrDesc,
					std::shared_ptr<spiht_encode_plan>& plan,
					std::shared_ptr<spiht_encode_action>& action,
					pQuery& qry);

void getSPIHTDecode(pArrayDesc sourceArrDesc,
					std::shared_ptr<spiht_decode_plan>& plan,
					std::shared_ptr<spiht_decode_action>& action,
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

void getSave(pArrayDesc sourceArrDesc,
			 std::shared_ptr<save_plan>& plan,
			 std::shared_ptr<save_action>& action,
			 pQuery& qry);

void getLoad(pArrayDesc sourceArrDesc,
			 std::shared_ptr<load_plan>& plan,
			 std::shared_ptr<load_action>& action,
			 pQuery& qry);

std::shared_ptr<load_plan> getLoadPlan(pArrayDesc sourceArrDesc, pQuery qry);

void getNaiveFilter(pArrayDesc sourceArrDesc,
					pPredicate myPredicate,
					std::shared_ptr<naive_filter_plan>& plan,
					std::shared_ptr<naive_filter_action>& action,
					pQuery& qry);

void getIndexFilter(pArrayDesc sourceArrDesc,
					pPredicate myPredicate,
					std::shared_ptr<index_filter_plan>& plan,
					std::shared_ptr<index_filter_action>& action,
					pQuery& qry);

template <typename plan_, typename action_, typename pset_>
std::tuple<std::shared_ptr<plan_>, std::shared_ptr<action_>, pQuery>
getOperator(parameters& params, pQuery myQuery = nullptr)
{
	if(!myQuery)
	{
		myQuery = std::make_shared<query>();
	}
	auto myPlan = std::make_shared<plan_>();
	myPlan->setParamSet(std::make_shared<pset_>(params));

	auto myAction = std::static_pointer_cast<action_>(myPlan->getAction());

	return std::make_tuple(myPlan, myAction, myQuery);
}
}		// caDummy
}		// msdb
#endif	// _MSDB_OPARRAYDUMMY_H_