#pragma once
#ifndef _MSDB_OPPLANDUMMY_H_
#define _MSDB_OPPLANDUMMY_H_

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
//////////////////////////////
// IO Operators
std::shared_ptr<save_plan> getSavePlan(pPlan sourcPlan, pQuery qry);
std::shared_ptr<load_plan> getLoadPlan(pArrayDesc sourceArrDesc, pQuery qry);

//////////////////////////////
// Wavelet Operators
//std::shared_ptr<wavelet_encode_plan> getWaveletEncodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry);
std::shared_ptr<wavelet_decode_plan> getWaveletDecodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry);

//std::shared_ptr<spiht_encode_plan> getSPIHTEncodePlan(pPlan sourcPlan, pQuery qry);
std::shared_ptr<spiht_decode_plan> getSPIHTDecodePlan(pArrayDesc sourceArrDesc, pQuery qry);

//////////////////////////////
// Se Compression Operators
//std::shared_ptr<se_compression_plan> getSeCompressionPlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry);
std::shared_ptr<se_decompression_plan> getSeDecompressionPlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry);

//std::shared_ptr<mmt_delta_encode_plan> getMMTDeltaEncodePlan(pPlan sourcPlan, pQuery qry);
std::shared_ptr<mmt_delta_decode_plan> getMMTDeltaDecodePlan(pPlan sourcPlan, pQuery qry);

//////////////////////////////
// Filter Operators
std::shared_ptr<naive_filter_plan> getNaiveFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry);
std::shared_ptr<index_filter_plan> getIndexFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry);

template <typename plan_, typename pset_>
std::shared_ptr<plan_>
getOpPlan(parameters& params, pQuery myQuery)
{
	auto myPlan = std::make_shared<plan_>();
	myPlan->setParamSet(std::make_shared<pset_>(params));

	return myPlan;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_OPPLANDUMMY_H_