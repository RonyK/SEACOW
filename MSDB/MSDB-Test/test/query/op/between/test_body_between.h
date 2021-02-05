#pragma once
#ifndef _MSDB_TESTBETWEEN_H_
#define _MSDB_TESTBETWEEN_H_

#include <pch.h>

#include <index/test_qry_mmt.h>
#include <io/test_action_io.h>
#include <compression/test_qry_spiht.h>
#include <compression/test_qry_secompression.h>
#include <compression/test_qry_zip.h>
#include <compression/test_qry_compass.h>

#include <op/between/between_plan.h>
#include <op/between/between_action.h>

namespace msdb
{
namespace caDummy
{
coorRange getRandomRange(const position_t dimX, const position_t dimY, const float selectivity);

// ##################################################
// # Test Body for Sequencial Random Between
template <typename value_type>
pArray test_body_seq_random_load_between(_pFuncGetSourceArray_,
										 _pFuncGetSourceArrayDesc_,
										 size_t numTests, std::vector<float> selectivities,
										 const position_t dimX, const position_t dimY,
										 bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString();
			test_body_seq_load_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												   qRange.getSp(), qRange.getEp(),
												   saveArray, validation, printFlag);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_spiht_between(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
										  eleDefault wtLevel,
										  size_t numTests, std::vector<float> selectivities,
										  const position_t dimX, const position_t dimY,
										  bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString();
			test_body_seq_spiht_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													wtLevel, qRange.getSp(), qRange.getEp(),
													saveArray, validation, printFlag);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_compass_between(_pFuncGetSourceArray_,
											_pFuncGetSourceArrayDesc_,
											eleDefault numBins,
											size_t numTests, std::vector<float> selectivities,
											const position_t dimX, const position_t dimY,
											bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString();
			test_body_seq_compass_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													  numBins,
													  qRange.getSp(), qRange.getEp(),
													  saveArray, validation, printFlag);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_zip_between(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										size_t numTests, std::vector<float> selectivities,
										const position_t dimX, const position_t dimY,
										bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString();
			test_body_seq_zip_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												  qRange.getSp(), qRange.getEp(),
												  saveArray, validation, printFlag);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_se_between(_pFuncGetSourceArray_,
									   _pFuncGetSourceArrayDesc_,
									   eleDefault wtLevel, eleDefault mmtLevel,
									   size_t numTests, std::vector<float> selectivities,
									   const position_t dimX, const position_t dimY,
									   bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString();
			test_body_seq_se_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												 wtLevel, mmtLevel,
												 qRange.getSp(), qRange.getEp(),
												 saveArray, validation, printFlag);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
	}
	//////////////////////////////

	return nullptr;
}
// ##################################################

// ##################################################
// # Test Body for Sequencial Between
template <typename value_type>
pArray test_body_seq_se_between(_pFuncGetSourceArray_,
								_pFuncGetSourceArrayDesc_,
								eleDefault wtLevel, eleDefault mmtLevel,
								coor sp, coor ep,
								bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray)
	{
		getSourceArrayIfEmpty(sourceArr);
	} else
	{
		getSourceArrayDesc(sourceArr);
	}
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_se_between<value_type>(sourceArr,
														 sp, ep,
														 wtLevel, mmtLevel,
														 saveArray, printFlag);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_spiht_between(_pFuncGetSourceArray_,
								   _pFuncGetSourceArrayDesc_,
								   eleDefault wtLevel,
								   coor sp, coor ep,
								   bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray)
	{
		getSourceArrayIfEmpty(sourceArr);
	} else
	{
		getSourceArrayDesc(sourceArr);
	}
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_spiht_between<value_type>(sourceArr,
															sp, ep,
															wtLevel,
															saveArray, printFlag);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_load_between(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  coor sp, coor ep,
								  bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray)
	{
		getSourceArrayIfEmpty(sourceArr);
	} else
	{
		getSourceArrayDesc(sourceArr);
	}
	sourceArr[0]->setId(sourceArr[0]->getId());
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_load_between<value_type>(sourceArr,
														   sp, ep,
														   saveArray, printFlag);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_compass_between(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault numBins,
									 coor sp, coor ep,
									 bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray)
	{
		getSourceArrayIfEmpty(sourceArr);
	} else
	{
		getSourceArrayDesc(sourceArr);
	}
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_compass_between<value_type>(sourceArr,
															  numBins,
															  sp, ep,
															  saveArray, printFlag);
	   //////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_zip_between(_pFuncGetSourceArray_,
								 _pFuncGetSourceArrayDesc_,
								 coor sp, coor ep,
								 bool saveArray = true, bool validation = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray)
	{
		getSourceArrayIfEmpty(sourceArr);
	} else
	{
		getSourceArrayDesc(sourceArr);
	}
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_zip_between<value_type>(sourceArr,
														  sp, ep,
														  saveArray, printFlag);
   //////////////////////////////

	return outArr;
}
// ##################################################

//////////////////////////////
// qBundles
//
template <typename value_type>
pArray exe_qbundle_seq_se_between(_vectorSourceArray_,
								  coor sp, coor ep,
								  eleDefault wtLevel,
								  eleDefault mmtLevel,
								  bool saveArray = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	}

	//////////////////////////////
	// 02. Between
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
									 bool saveArray = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_spiht_between<value_type>(sourceArr,
												 sp, ep,
												 wtLevel,
												 printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_load_between(_vectorSourceArray_,
									coor sp, coor ep,
									bool saveArray = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_save<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_load_between<value_type>(sourceArr,
												sp, ep,
												printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_compass_between(_vectorSourceArray_,
									   eleDefault numBins,
									   coor sp, coor ep,
									   bool saveArray = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, printFlag);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_compass_between<value_type>(sourceArr,
												   numBins,
												   sp, ep,
												   printFlag);
	  //////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_zip_between(_vectorSourceArray_,
								   coor sp, coor ep,
								   bool saveArray = true, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_zip_save<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_zip_between<value_type>(sourceArr,
											   sp, ep,
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
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Delta Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_between(_vectorSourceArray_,
								 coor sp, coor ep,
								 eleDefault wtLevel,
								 bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto spihtPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spihtPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtDecodePlan, sp, ep, qry);

	auto outArr = spihtPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_between(_vectorSourceArray_,
								coor sp, coor ep,
								bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Load Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_between(_vectorSourceArray_,
								   eleDefault numBins,
								   coor sp, coor ep,
								   bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto decodePlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);
	auto betweenPlan = getBetweenPlan(decodePlan, sp, ep, qry);

	auto outArr = decodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Compass Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_zip_between(_vectorSourceArray_,
							   coor sp, coor ep,
							   bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getZipLoadPlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Zip Load Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TESTBETWEEN_H_