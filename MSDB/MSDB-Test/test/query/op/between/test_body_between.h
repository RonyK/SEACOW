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
#include <compression/test_qry_lzw.h>

#include <op/between/between_plan.h>
#include <op/between/between_action.h>

#include <query/op/huffman/qry_huffman_encode_decode.h>
#include <query/op/adapt_huffman/qry_adapt_huffman_encode_decode.h>
#include <query/op/lzw_huffman/qry_lzw_huffman_encode_decode.h>

#include <exp/experimentsInfo.h>

namespace msdb
{
namespace caDummy
{
coorRange getRandomRange(const position_t dimX, const position_t dimY, const float selectivity);

void getOpBetween(pArrayDesc sourceArrDesc, std::shared_ptr<between_plan>& plan, std::shared_ptr<between_action>& action, pQuery& qry, coor sp, coor ep);

pArray exe_act_ind_raw_between(std::vector<pArray> sourceArr, coor sp, coor ep);

template <typename value_type>
pArray exe_qry_ind_raw_between(_vectorSourceArray_, coor sp, coor ep,
							   bool printFlag = false)
{
	auto outArr = exe_act_ind_raw_between(sourceArr, sp, ep);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Raw Betwee Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_raw_between(_pFuncGetSourceArray_,
								_pFuncGetSourceArrayDesc_,
								coor sp, coor ep,
								bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId());

	auto outArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep, printFlag);

	return outArr;
}

// ##################################################
// # Test Body for Sequencial Random Between
template <typename value_type>
pArray test_body_seq_random_load_between(_pFuncGetSourceArray_,
										 _pFuncGetSourceArrayDesc_,
										 size_t numTests, std::vector<float> selectivities,
										 const position_t dimX, const position_t dimY,
										 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_load_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												   qRange.getSp(), qRange.getEp(),
												   saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
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
										  bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_spiht_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													wtLevel, qRange.getSp(), qRange.getEp(),
													saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
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
											bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_compass_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													  numBins,
													  qRange.getSp(), qRange.getEp(),
													  saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_zip_between(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										size_t numTests, std::vector<float> selectivities,
										const position_t dimX, const position_t dimY,
										bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_zip_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												  qRange.getSp(), qRange.getEp(),
												  saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_lzw_between(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										size_t numTests, std::vector<float> selectivities,
										const position_t dimX, const position_t dimY,
										bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_lzw_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												  qRange.getSp(), qRange.getEp(),
												  saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_huffman_between(_pFuncGetSourceArray_,
											_pFuncGetSourceArrayDesc_,
											size_t numTests, std::vector<float> selectivities,
											const position_t dimX, const position_t dimY,
											bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_huffman_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													  qRange.getSp(), qRange.getEp(),
													  saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_adapt_huffman_between(_pFuncGetSourceArray_,
												  _pFuncGetSourceArrayDesc_,
												  size_t numTests, std::vector<float> selectivities,
												  const position_t dimX, const position_t dimY,
												  bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_adapt_huffman_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
															qRange.getSp(), qRange.getEp(),
															saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_random_lzw_huffman_between(_pFuncGetSourceArray_,
												_pFuncGetSourceArrayDesc_,
												size_t numTests, std::vector<float> selectivities,
												const position_t dimX, const position_t dimY,
												bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_lzw_huffman_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
														  qRange.getSp(), qRange.getEp(),
														  saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
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
									   bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(rangeSeed);
	auto tempQ = std::make_shared<query>();	// Not used, just for log
	//////////////////////////////

	//////////////////////////////
	// 02. Build MMT
	try
	{
		attributeId attrId = 0;
		std::vector<pArray> sourceArr;
		getSourceArrayDesc(sourceArr);

		arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), attrId);
	}
	catch (msdb_exception e)
	{
		test_body_mmt_build<value_type>(getSourceArrayIfEmpty, mmtLevel, false);
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Range : " << qRange.toString() << "(" << selectivity << ")";

			test_body_seq_se_between<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												 wtLevel, mmtLevel,
												 qRange.getSp(), qRange.getEp(),
												 saveArray, validation, printFlag, j * numTests + i);
			BOOST_LOG_TRIVIAL(info) << "##################################################";
		}
		++j;
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
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_se_between<value_type>(sourceArr,
														 sp, ep,
														 wtLevel, mmtLevel,
														 saveArray, printFlag,
														 experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_spiht_between(_pFuncGetSourceArray_,
								   _pFuncGetSourceArrayDesc_,
								   eleDefault wtLevel,
								   coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_spiht_between<value_type>(sourceArr,
															sp, ep,
															wtLevel,
															saveArray, printFlag,
															experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_load_between(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId());
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_load_between<value_type>(sourceArr,
														   sp, ep,
														   saveArray, printFlag,
														   experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_compass_between(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault numBins,
									 coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_compass_between<value_type>(sourceArr,
															  numBins,
															  sp, ep,
															  saveArray, printFlag,
															  experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_zip_between(_pFuncGetSourceArray_,
								 _pFuncGetSourceArrayDesc_,
								 coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_zip_between<value_type>(sourceArr,
														  sp, ep,
														  saveArray, printFlag,
														  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_between(_pFuncGetSourceArray_,
								 _pFuncGetSourceArrayDesc_,
								 coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_lzw_between<value_type>(sourceArr,
														  sp, ep,
														  saveArray, printFlag,
														  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_huffman_between(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_huffman_between<value_type>(sourceArr,
															  sp, ep,
															  saveArray, printFlag,
															  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_adapt_huffman_between(_pFuncGetSourceArray_,
										   _pFuncGetSourceArrayDesc_,
										   coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + adapt_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_adapt_huffman_between<value_type>(sourceArr,
																	sp, ep,
																	saveArray, printFlag,
																	experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_huffman_between(_pFuncGetSourceArray_,
										 _pFuncGetSourceArrayDesc_,
										 coor sp, coor ep,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Between
	auto outArr = exe_qbundle_seq_lzw_huffman_between<value_type>(sourceArr,
																  sp, ep,
																  saveArray, printFlag,
																  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 03. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

// ##################################################

template <typename value_type>
pArray test_body_seq_se_between(std::vector<pArray> sourceArr,
								std::vector<pArray> sourceArrDesc,
								eleDefault wtLevel, eleDefault mmtLevel,
								coor sp, coor ep,
								bool saveArray = false, bool validation = false, bool printFlag = false,
								size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_se_between<value_type>(sourceArrDesc,
														 sp, ep,
														 wtLevel, mmtLevel,
														 saveArray, printFlag,
														 experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_spiht_between(std::vector<pArray> sourceArr,
								   std::vector<pArray> sourceArrDesc,
								   eleDefault wtLevel,
								   coor sp, coor ep,
								   bool saveArray = false, bool validation = false, bool printFlag = false,
								   size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_spiht_between<value_type>(sourceArr,
															sp, ep,
															wtLevel,
															saveArray, printFlag,
															experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_load_between(std::vector<pArray> sourceArr,
								  std::vector<pArray> sourceArrDesc,
								  coor sp, coor ep,
								  bool saveArray = false, bool validation = false, bool printFlag = false,
								  size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_load_between<value_type>(sourceArr,
														   sp, ep,
														   saveArray, printFlag,
														   experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_compass_between(std::vector<pArray> sourceArr,
									 std::vector<pArray> sourceArrDesc,
									 eleDefault numBins,
									 coor sp, coor ep,
									 bool saveArray = false, bool validation = false, bool printFlag = false,
									 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_compass_between<value_type>(sourceArr,
															  numBins,
															  sp, ep,
															  saveArray, printFlag,
															  experiments::between_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_zip_between(std::vector<pArray> sourceArr,
								 std::vector<pArray> sourceArrDesc,
								 coor sp, coor ep,
								 bool saveArray = false, bool validation = false, bool printFlag = false,
								 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_zip_between<value_type>(sourceArr,
														  sp, ep,
														  saveArray, printFlag,
														  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_between(std::vector<pArray> sourceArr,
								 std::vector<pArray> sourceArrDesc,
								 coor sp, coor ep,
								 bool saveArray = false, bool validation = false, bool printFlag = false,
								 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Between
	auto outArr = exe_qbundle_seq_lzw_between<value_type>(sourceArr,
														  sp, ep,
														  saveArray, printFlag,
														  experiments::between_random::expId, expTrial);
	//////////////////////////////

	 //////////////////////////////
	 // 02. Validation
	if (validation)
	{
		auto rawArr = exe_qry_ind_raw_between<value_type>(sourceArr, sp, ep);
		compArrary<value_type>(outArr, rawArr);
	}
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
								  bool saveArray = false, bool printFlag = false,
								  size_t expId = 0, size_t expTrial = 0)
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
											  printFlag,
											  expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_spiht_between(_vectorSourceArray_,
									 coor sp, coor ep,
									 eleDefault wtLevel,
									 bool saveArray = false, bool printFlag = false,
									 size_t expId = 0, size_t expTrial = 0)
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
												 printFlag,
												 expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_load_between(_vectorSourceArray_,
									coor sp, coor ep,
									bool saveArray = false, bool printFlag = false,
									size_t expId = 0, size_t expTrial = 0)
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
												printFlag,
												expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_compass_between(_vectorSourceArray_,
									   eleDefault numBins,
									   coor sp, coor ep,
									   bool saveArray = false, bool printFlag = false,
									   size_t expId = 0, size_t expTrial = 0)
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
												   printFlag,
												   expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_zip_between(_vectorSourceArray_,
								   coor sp, coor ep,
								   bool saveArray = false, bool printFlag = false,
								   size_t expId = 0, size_t expTrial = 0)
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
											   printFlag,
											   expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_lzw_between(_vectorSourceArray_,
								   coor sp, coor ep,
								   bool saveArray = false, bool printFlag = false,
								   size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_lzw_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_lzw_between<value_type>(sourceArr,
											   sp, ep,
											   printFlag,
											   expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_huffman_between(_vectorSourceArray_,
									   coor sp, coor ep,
									   bool saveArray = false, bool printFlag = false,
									   size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_huffman_between<value_type>(sourceArr,
												   sp, ep,
												   printFlag,
												   expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_adapt_huffman_between(_vectorSourceArray_,
											 coor sp, coor ep,
											 bool saveArray = false, bool printFlag = false,
											 size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_adapt_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_adapt_huffman_between<value_type>(sourceArr,
														 sp, ep,
														 printFlag,
														 expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_lzw_huffman_between(_vectorSourceArray_,
										   coor sp, coor ep,
										   bool saveArray = false, bool printFlag = false,
										   size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_lzw_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 02. Between
	return exe_qry_seq_lzw_huffman_between<value_type>(sourceArr,
													   sp, ep,
													   printFlag,
													   expId, expTrial);
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
							  bool printFlag = false,
							  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	//auto deltaDecodePlan = getMMTDeltaDecodePlan(seDecompPlan, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtDecodePlan, sp, ep, qry);

	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	//outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//if (printFlag)
	//{
	//	BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
	//	BOOST_LOG_TRIVIAL(info) << "Delta Decode Arr" << std::endl;
	//	outArr->print();
	//	//outArr->getChunkBitmap()->print();
	//}

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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_between(_vectorSourceArray_,
								 coor sp, coor ep,
								 eleDefault wtLevel,
								 bool printFlag = false,
								 size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto spihtPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spihtPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtDecodePlan, sp, ep, qry);

	BOOST_LOG_TRIVIAL(debug) << "execute: spiht start" << std::endl;
	auto outArr = spihtPlan->getAction()->execute(sourceArr, qry);
	BOOST_LOG_TRIVIAL(debug) << "execute: spiht end" << std::endl;
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	BOOST_LOG_TRIVIAL(debug) << "execute: wtDecode start" << std::endl;
	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	BOOST_LOG_TRIVIAL(debug) << "execute: wtDecode end" << std::endl;
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	BOOST_LOG_TRIVIAL(debug) << "execute: betweenPlan start" << std::endl;
	outArr = betweenPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	BOOST_LOG_TRIVIAL(debug) << "execute: betweenPlan end" << std::endl;
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Between Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_between(_vectorSourceArray_,
								coor sp, coor ep,
								bool printFlag = false,
								size_t expId = 0, size_t expTrial = 0)
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_between(_vectorSourceArray_,
								   eleDefault numBins,
								   coor sp, coor ep,
								   bool printFlag = false,
								   size_t expId = 0, size_t expTrial = 0)
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_zip_between(_vectorSourceArray_,
							   coor sp, coor ep,
							   bool printFlag = false, size_t expId = 0, size_t expTrial = 0)
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_lzw_between(_vectorSourceArray_,
							   coor sp, coor ep,
							   bool printFlag = false, size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLzwDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "LZW Decode Arr" << std::endl;
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_huffman_between(_vectorSourceArray_,
								   coor sp, coor ep,
								   bool printFlag = false, size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Huffman Decode Arr" << std::endl;
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_adapt_huffman_between(_vectorSourceArray_,
										 coor sp, coor ep,
										 bool printFlag = false, size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getAdaptHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Adapt Huffman Decode Arr" << std::endl;
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_lzw_huffman_between(_vectorSourceArray_,
									   coor sp, coor ep,
									   bool printFlag = false, size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLzwHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Lzw Huffman Decode Arr" << std::endl;
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

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TESTBETWEEN_H_