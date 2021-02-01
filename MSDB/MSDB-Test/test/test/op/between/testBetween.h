#pragma once
#ifndef _MSDB_TESTBETWEEN_H_
#define _MSDB_TESTBETWEEN_H_

#include <pch.h>

#include <index/test_qry_mmt.h>
#include <io/test_action_io.h>
#include <compression/test_qry_spiht.h>
#include <compression/test_qry_secompression.h>

#include <op/between/between_plan.h>
#include <op/between/between_action.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_seq_se_between(_pFuncGetSourceArray_,
								_pFuncGetSourceArrayDesc_,
								eleDefault wtLevel, eleDefault mmtLevel,
								coor sp, coor ep, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 3);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_se_between<value_type>(sourceArr,
														 sp, ep,
														 wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_spiht_between(_pFuncGetSourceArray_,
								   _pFuncGetSourceArrayDesc_,
								   eleDefault wtLevel, eleDefault mmtLevel,
								   coor sp, coor ep, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 3);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_spiht_between<value_type>(sourceArr,
															sp, ep,
															wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_load_between(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  eleDefault wtLevel, eleDefault mmtLevel,
								  coor sp, coor ep, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 3);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_load_between<value_type>(sourceArr,
															sp, ep,
															wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	return outArr;
}

//////////////////////////////
// qBundles
//
template <typename value_type>
pArray exe_qbundle_seq_se_between(_vectorSourceArray_,
								  coor sp, coor ep,
								  eleDefault wtLevel,
								  eleDefault mmtLevel,
								  bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
	exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_se_between<value_type>(sourceArr,
											  sp, ep,
											  wtLevel, mmtLevel,
											  printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_spiht_between(_vectorSourceArray_,
									 coor sp, coor ep,
									 eleDefault wtLevel,
									 eleDefault mmtLevel,
									 bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, false);

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_spiht_between<value_type>(sourceArr,
												 sp, ep,
												 wtLevel, mmtLevel,
												 printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_load_between(_vectorSourceArray_,
									coor sp, coor ep,
									eleDefault wtLevel,
									eleDefault mmtLevel,
									bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_save<value_type>(sourceArr, false);

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_load_between<value_type>(sourceArr,
												sp, ep,
												wtLevel, mmtLevel,
												printFlag);
	//////////////////////////////
}

//////////////////////////////
// Queries
//
template <typename value_type>
pArray exe_qry_seq_se_between(_vectorSourceArray_,
							  coor sp, coor ep,
							  eleDefault wtLevel,
							  eleDefault mmtLevel,
							  bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto deltaDecodePlan = getMMTDeltaDecodePlan(wtDecodePlan, qry);
	auto betweenPlan = getBetweenPlan(deltaDecodePlan, sp, ep, qry);

	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decomp Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wt Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Delta Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Between Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_between(_vectorSourceArray_,
								 coor sp, coor ep,
								 eleDefault wtLevel,
								 eleDefault mmtLevel,
								 bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto spihtPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spihtPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtDecodePlan, sp, ep, qry);

	auto outArr = spihtPlan->getAction()->execute(sourceArr, qry);
	outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decomp Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wt Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Between Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_between(_vectorSourceArray_,
								coor sp, coor ep,
								eleDefault wtLevel,
								eleDefault mmtLevel,
								bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Load Arr" << std::endl;
		outArr->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Between Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TESTBETWEEN_H_