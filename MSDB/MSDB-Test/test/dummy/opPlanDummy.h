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
	//std::shared_ptr<load_plan> getLoadPlan(pPlan sourcPlan, pQuery qry);
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