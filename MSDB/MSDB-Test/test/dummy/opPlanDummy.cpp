#include <pch.h>

namespace msdb
{
namespace core
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

std::shared_ptr<zip_save_plan> getZipSavePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<zip_save_plan, zip_save_array_pset>(sourceArrDesc, qry);
}

std::shared_ptr<zip_load_plan> getZipLoadPlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<zip_load_plan, zip_load_array_pset>(sourceArrDesc, qry);
}

std::shared_ptr<compass_encode_plan> getCompassEncodePlan(pArrayDesc sourceArrDesc, eleDefault numBins, pQuery qry)
{
	return getLevelArrayParamPlan<compass_encode_plan, compass_encode_array_pset>(sourceArrDesc, numBins, qry);
}

std::shared_ptr<compass_decode_plan> getCompassDecodePlan(pArrayDesc sourceArrDesc, eleDefault numBins, pQuery qry)
{
	return getLevelArrayParamPlan<compass_decode_plan, compass_decode_array_pset>(sourceArrDesc, numBins, qry);
}

std::shared_ptr<lzw_encode_plan> getLzwEncodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<lzw_encode_plan, lzw_encode_array_pset>(sourceArrDesc, qry);
}

std::shared_ptr<lzw_decode_plan> getLzwDecodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<lzw_decode_plan, lzw_decode_array_pset>(sourceArrDesc, qry);
}

//////////////////////////////
// Wavelet Operators
std::shared_ptr<wavelet_encode_plan> getWaveletEncodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry)
{
	return getLevelPlanParamPlan<wavelet_encode_plan, wavelet_encode_plan_pset>(sourcPlan, wtLevel, qry);
}

std::shared_ptr<wavelet_decode_plan> getWaveletDecodePlan(pPlan sourcPlan, eleDefault wtLevel, pQuery qry)
{
	return getLevelPlanParamPlan<wavelet_decode_plan, wavelet_decode_plan_pset>(sourcPlan, wtLevel, qry);
}

std::shared_ptr<spiht_encode_plan> getSPIHTEncodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<spiht_encode_plan, spiht_encode_array_pset>(sourceArrDesc, wtLevel, qry);
}

std::shared_ptr<spiht_decode_plan> getSPIHTDecodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<spiht_decode_plan, spiht_decode_array_pset>(sourceArrDesc, wtLevel, qry);
}

//////////////////////////////
// Se Compression Operators
std::shared_ptr<se_compression_plan> getSeCompressionPlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<se_compression_plan, se_compression_array_pset>(sourceArrDesc, wtLevel, qry);
}

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

std::shared_ptr<mmt_load_plan> getMMTLoadPlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<mmt_load_plan, mmt_load_array_pset>(sourceArrDesc, qry);
}

//////////////////////////////
// Filter Operators
std::shared_ptr<naive_filter_plan> getNaiveFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<naive_filter_plan, naive_filter_plan_pset>(sourcPlan, inPredicate, qry);
}
std::shared_ptr<index_filter_plan> getIndexFilterPlan(pPlan sourcPlan, pPredicate inPredicate, pQuery qry)
{
	return getPredicatePlanParamPlan<index_filter_plan, index_filter_plan_pset>(sourcPlan, inPredicate, qry);
}

//////////////////////////////
// Between Operators
std::shared_ptr<between_plan> getBetweenPlan(pPlan sourcPlan, coor sp, coor ep, pQuery qry)
{
	return getPlan_setPlanRange<between_plan, between_plan_pset>(sourcPlan, sp, ep, qry);
}
}		// caDummy
}		// core
}		// msdb