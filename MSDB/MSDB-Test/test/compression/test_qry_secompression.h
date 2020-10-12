#pragma once
#ifndef _MSDB_TEST_ACTION_SECOMPRESSION_H_
#define _MSDB_TEST_ACTION_SECOMPRESSION_H_

#include <pch.h>
#include <index/test_action_mmt.h>
#include <util/timer.h>
#include <dummy/opDummy.h>
#include <compression/test_action_compression.h>

namespace msdb
{
namespace caDummy
{

template <typename value_type>
pArray exe_qry_ind_se_compression(_vectorSourceArray_,
								  eleDefault wtLevel, eleDefault mmtLevel,
								  bool printFlag = false)
{
	auto outArr = exe_act_ind_mmt_delta_encode(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Delta Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_encode(std::vector<pArray>({ outArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Encode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_se_compression(std::vector<pArray>({ outArr }));
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SE Compression Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_se_decompression(_vectorSourceArray_,
									eleDefault wtLevel, eleDefault mmtLevel,
									bool printFlag = false)
{
	auto outArr = exe_act_ind_se_decompression(sourceArr, wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_decode(std::vector<pArray>({ outArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_mmt_delta_decode(std::vector<pArray>({ outArr }));
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Delta Decode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_decompression(_vectorSourceArray_,
									eleDefault wtLevel, eleDefault mmtLevel,
									bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto deltaDecodePlan = getMMTDeltaDecodePlan(wtDecodePlan, qry);

	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decompression Arr" << std::endl;
		outArr->print();
	}


	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Delta Decode Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_delta_spiht_encode(_vectorSourceArray_,
									  eleDefault wtLevel, eleDefault mmtLevel,
									  bool printFlag = false)
{
	auto outArr = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	if (false)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_mmt_delta_encode(std::vector<pArray>({ outArr }));
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Delta Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_encode(std::vector<pArray>({ outArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Encode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_spiht_encode(std::vector<pArray>({ outArr }));
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SPIHT Encode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_delta_spiht_decode(_vectorSourceArray_,
									  eleDefault wtLevel, eleDefault mmtLevel,
									  bool printFlag = false)
{
	auto outArr = exe_act_ind_spiht_decode(sourceArr, wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SPIHT Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_decode(std::vector<pArray>({ outArr }), wtLevel);
	std::cout << "##############################" << std::endl;
	std::cout << "Wavelet Decode Arr" << std::endl;
	if (printFlag)
	{
		outArr->print();
	}

	outArr = exe_act_ind_mmt_delta_decode(std::vector<pArray>({ outArr }));
	std::cout << "##############################" << std::endl;
	std::cout << "Delta Decode Arr" << std::endl;
	if (printFlag)
	{
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qbundle_ind_se_comp(_vectorSourceArray_,
						   eleDefault wtLevel, eleDefault mmtLevel,
						   bool printFlag = false)
{
	//////////////////////////////
	// 01. Build MMT
	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);

	//////////////////////////////
	// 02. Save SE Array
	return exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
}

template <typename value_type>
pArray test_body_ind_se_comp_decomp(_pFuncGetSourceArray_,
									_pFuncGetSourceArrayDesc_,
									eleDefault wtLevel, eleDefault mmtLevel,
									bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
	exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = exe_qry_ind_se_decompression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	//////////////////////////////
	// 04. Evaluation
	compArrary<value_type>(sourceArr[0], outArr);
	//////////////////////////////
	return outArr;
}


template <typename value_type>
pArray test_body_ind_se_comp(_pFuncGetSourceArray_,
							_pFuncGetSourceArrayDesc_,
							eleDefault wtLevel, eleDefault mmtLevel, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	return exe_qbundle_ind_se_comp<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray test_body_seq_se_comp_decomp(_pFuncGetSourceArray_,
									_pFuncGetSourceArrayDesc_,
									eleDefault wtLevel,
									eleDefault mmtLevel,
									bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	exe_qbundle_ind_se_comp<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Decompression Array
	auto outArr = exe_qry_seq_se_decompression<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 04. Evaluation
	compArrary<value_type>(sourceArr[0], outArr);
	//////////////////////////////

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TESTSECOMPRESSION_H_