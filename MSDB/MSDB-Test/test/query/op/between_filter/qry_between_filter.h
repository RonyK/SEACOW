#pragma once
#ifndef _MSDB_TEST_QRY_BETWEEN_INDEX_H_
#define _MSDB_TEST_QRY_BETWEEN_INDEX_H_

#include <pch.h>
#include <dummy/opDummy.h>
#include <dummy/equalTest.h>
#include <exp/experimentsInfo.h>
#include <io/test_action_io.h>
#include <query/op/between/test_body_between.h>
#include <query/op/filter/test_body_filter.h>

namespace msdb
{
namespace caDummy
{
// ##################################################
template <typename value_type>
pArray exe_qry_seq_load_between_filter(_vectorSourceArray_,
									   coor sp, coor ep,
									   pPredicate inPredicate,
									   bool printFlag = false,
									   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Load Arr" << std::endl;
		outArr->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}
// ##################################################

// ##################################################
template <typename value_type>
pArray test_body_seq_load_between_filter(_pFuncGetSourceArray_,
										 _pFuncGetSourceArrayDesc_,
										 coor sp, coor ep,
										 int64_t value,
										 bool saveArray = false, bool validation = false, bool printFlag = false,
										 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
	{
		getSourceArrayIfEmpty(sourceArr);
	}
	else
	{
		getSourceArrayDesc(sourceArr);
	}
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_save<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_load_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
															  printFlag,
															  experiments::between_filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 04. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}
// ##################################################

// ##################################################
template <typename value_type>
pArray test_body_seq_load_random_between_filter(_pFuncGetSourceArray_,
												_pFuncGetSourceArrayDesc_,
												size_t numTests, std::vector<float> selectivities,
												const position_t dimX, const position_t dimY,
												bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			v = (value_type)rand();

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_load_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
														  qRange.getSp(), qRange.getEp(),
														  v, saveArray, validation, printFlag,
														  j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}		// TEST()
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_QRY_BETWEEN_INDEX_H_