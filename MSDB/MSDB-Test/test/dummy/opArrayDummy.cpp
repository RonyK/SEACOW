#include "pch.h"
#include "opArrayDummy.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
void getWaveletEncode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_encode_plan>& plan,
					  std::shared_ptr<wavelet_encode_action>& action,
					  pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<wavelet_encode_plan, wavelet_encode_action, wavelet_encode_array_pset>(
		sourceArrDesc, level, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getWaveletDecode(pArrayDesc sourceArrDesc, eleDefault level,
					  std::shared_ptr<wavelet_decode_plan>& plan,
					  std::shared_ptr<wavelet_decode_action>& action,
					  pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<wavelet_decode_plan, wavelet_decode_action, wavelet_decode_array_pset>(
		sourceArrDesc, level, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getMmtBuild(pArrayDesc sourceArrDesc,
				 eleDefault& level,
				 std::shared_ptr<mmt_build_plan>& plan,
				 std::shared_ptr<mmt_build_action>& action,
				 pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<mmt_build_plan, mmt_build_action, mmt_build_array_pset>(
		sourceArrDesc, level, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getMmtSave(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_save_plan>& plan,
				std::shared_ptr<mmt_save_action>& action,
				pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<mmt_save_plan, mmt_save_action, mmt_save_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getMmtLoad(pArrayDesc sourceArrDesc,
				std::shared_ptr<mmt_load_plan>& plan,
				std::shared_ptr<mmt_load_action>& action,
				pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<mmt_load_plan, mmt_load_action, mmt_load_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getMmtDeltaEncode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_encode_plan>& plan,
					   std::shared_ptr<mmt_delta_encode_action>& action,
					   pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<mmt_delta_encode_plan, mmt_delta_encode_action, mmt_delta_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getMmtDeltaDecode(pArrayDesc sourceArrDesc,
					   std::shared_ptr<mmt_delta_decode_plan>& plan,
					   std::shared_ptr<mmt_delta_decode_action>& action,
					   pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<mmt_delta_decode_plan, mmt_delta_decode_action, mmt_delta_decode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getSeCompression(pArrayDesc sourceArrDesc,
					  std::shared_ptr<se_compression_plan>& plan,
					  std::shared_ptr<se_compression_action>& action,
					  pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<se_compression_plan, se_compression_action, se_compression_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getSeDecompression(pArrayDesc sourceArrDesc, eleDefault level,
						std::shared_ptr<se_decompression_plan>& plan,
						std::shared_ptr<se_decompression_action>& action,
						pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<se_decompression_plan, se_decompression_action, se_decompression_array_pset>(
		sourceArrDesc, level, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getCompassEncode(pArrayDesc sourceArrDesc, eleDefault numBins,
					  std::shared_ptr<compass_encode_plan>& plan,
					  std::shared_ptr<compass_encode_action>& action,
					  pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<compass_encode_plan, compass_encode_action, compass_encode_array_pset>(
		sourceArrDesc, numBins, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getCompassDecode(pArrayDesc sourceArrDesc, eleDefault numBins,
					  std::shared_ptr<compass_decode_plan>& plan,
					  std::shared_ptr<compass_decode_action>& action,
					  pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<compass_decode_plan, compass_decode_action, compass_decode_array_pset>(
		sourceArrDesc, numBins, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getCompassIndexBuild(pArrayDesc sourceArrDesc, eleDefault numBins,
						  std::shared_ptr<compass_index_build_plan>& plan,
						  std::shared_ptr<compass_index_build_action>& action,
						  pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<compass_index_build_plan, compass_index_build_action, compass_index_build_array_pset>(
		sourceArrDesc, numBins, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

//std::shared_ptr<se_decompression_plan> getSeDecompression(pArrayDesc sourceArrDesc, eleDefault level,
//						pQuery& qry)
//{
//	return getLevelArrayParamPlan<se_decompression_plan, se_decompression_array_pset>(
//		sourceArrDesc, level, qry);
//}
void getSave(pArrayDesc sourceArrDesc,
			 std::shared_ptr<save_plan>& plan,
			 std::shared_ptr<save_action>& action,
			 pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<save_plan, save_action, save_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getLoad(pArrayDesc sourceArrDesc,
			 std::shared_ptr<load_plan>& plan,
			 std::shared_ptr<load_action>& action,
			 pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<load_plan, load_action, load_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getNaiveFilter(pArrayDesc sourceArrDesc,
					pPredicate myPredicate,
					std::shared_ptr<naive_filter_plan>& plan,
					std::shared_ptr<naive_filter_action>& action,
					pQuery& qry)
{
	auto planAction = getPredicateArrayParamOperator<naive_filter_plan, naive_filter_action, naive_filter_array_pset>(
		sourceArrDesc, myPredicate, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getIndexFilter(pArrayDesc sourceArrDesc,
					pPredicate myPredicate,
					std::shared_ptr<index_filter_plan>& plan,
					std::shared_ptr<index_filter_action>& action,
					pQuery& qry)
{
	auto planAction = getPredicateArrayParamOperator<index_filter_plan, index_filter_action, index_filter_array_pset>(
		sourceArrDesc, myPredicate, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getSPIHTEncode(pArrayDesc sourceArrDesc,
					std::shared_ptr<spiht_encode_plan>& plan,
					std::shared_ptr<spiht_encode_action>& action,
					pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<spiht_encode_plan, spiht_encode_action, spiht_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

void getSPIHTDecode(pArrayDesc sourceArrDesc,
					eleDefault wtLevel,
					std::shared_ptr<spiht_decode_plan>& plan,
					std::shared_ptr<spiht_decode_action>& action,
					pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<spiht_decode_plan, spiht_decode_action, spiht_decode_array_pset>(
		sourceArrDesc, wtLevel, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
}		// caDummy
}		// core
}		// msdb
