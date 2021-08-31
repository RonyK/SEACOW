#pragma once
#ifndef _MSDB_TESTSPIHT_H_
#define _MSDB_TESTSPIHT_H_

#include <pch.h>
#include <compression/test_action_compression.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
template <typename value_type>
pArray exe_qry_ind_spiht_encode(_vectorSourceArray_,
								eleDefault wtLevel,
								bool printFlag = false)
{
	auto outArr = exe_act_ind_wavelet_encode(sourceArr, wtLevel);
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
pArray exe_qry_ind_spiht_decode(_vectorSourceArray_,
								eleDefault wtLevel,
								bool printFlag = false)
{
	auto outArr = exe_act_ind_spiht_decode(std::vector<pArray>({ sourceArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SPIHT Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_decode(std::vector<pArray>({ outArr }), wtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Decode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_decode(_vectorSourceArray_,
								eleDefault wtLevel,
								bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto spDecodePlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spDecodePlan, wtLevel, qry);

	auto outArr = spDecodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SPIHT Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wavelet Decode Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}
}		// caDummy
}		// core
}		// msdb
#endif	//_MSDB_TESTSPIHT_H_