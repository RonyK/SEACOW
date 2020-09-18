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
}		// caDummy
}		// msdb
#endif	// _MSDB_TESTSECOMPRESSION_H_