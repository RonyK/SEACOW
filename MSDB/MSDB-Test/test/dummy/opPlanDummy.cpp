#include <pch.h>

namespace msdb
{
namespace caDummy
{
//////////////////////////////
// IO Operators
std::shared_ptr<save_plan> getSavePlan(pPlan sourcPlan, pQuery qry)
{
	return getSinglePlanParamPlan<save_plan, save_array_pset>(sourcPlan, qry);
}
std::shared_ptr<load_plan> getLoadPlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<load_plan, load_array_pset>(sourceArrDesc, qry);
}

//////////////////////////////
// Wavelet Operators
//std::shared_ptr<wavelet_encode_plan> getWaveletEncodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry)
//{
//	return getLevelPlanParamPlan<wavelet_encode_plan, wavelet_encode_plan_pset>(sourcPlan, wtLevel, qry);
//}

std::shared_ptr<wavelet_decode_plan> getWaveletDecodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry)
{
	return getLevelPlanParamPlan<wavelet_decode_plan, wavelet_decode_plan_pset>(sourcPlan, wtLevel, qry);
}

//std::shared_ptr<spiht_encode_plan> getSPIHTEncodePlan(pPlan sourcPlan, pQuery qry)
//{
//	return std::shared_ptr<spiht_encode_plan>();
//}

std::shared_ptr<spiht_decode_plan> getSPIHTDecodePlan(pPlan sourcPlan, pQuery qry)
{
	return getSinglePlanParamPlan<spiht_decode_plan, spiht_decode_plan_pset>(sourcPlan, qry);
}

//////////////////////////////
// Se Compression Operators
//std::shared_ptr<se_compression_plan> getSeCompressionPlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry)
//{
//	return std::shared_ptr<se_compression_plan>();
//}

std::shared_ptr<se_decompression_plan> getSeDecompressionPlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<se_decompression_plan, se_decompression_array_pset>(sourceArrDesc, wtLevel, qry);
}

//std::shared_ptr<mmt_delta_encode_plan> getMMTDeltaEncodePlan(pPlan sourcPlan, pQuery qry)
//{
//	return std::shared_ptr<mmt_delta_encode_plan>();
//}

std::shared_ptr<mmt_delta_decode_plan> getMMTDeltaDecodePlan(pPlan sourcPlan, pQuery qry)
{
	return getSinglePlanParamPlan<mmt_delta_decode_plan, mmt_delta_decode_plan_pset>(sourcPlan, qry);
}


std::shared_ptr<naive_filter_plan> getNaiveFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<naive_filter_plan, naive_filter_plan_pset>(sourcPlan, inPredicate, qry);
}
std::shared_ptr<index_filter_plan> getIndexFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<index_filter_plan, index_filter_plan_pset>(sourcPlan, inPredicate, qry);
}
}	// caDummy
}	// msdb