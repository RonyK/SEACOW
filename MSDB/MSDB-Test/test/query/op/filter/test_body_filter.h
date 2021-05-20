#pragma once
#ifndef _MSDB_TESTFILTER_H_
#define _MSDB_TESTFILTER_H_

#include <pch.h>
#include <array/attributeId.h>
#include <parse/predicate.h>
#include <index/test_action_mmt.h>
#include <io/test_action_io.h>
#include <index/test_qry_mmt.h>

#include <compression/test_qry_secompression.h>
#include <compression/test_qry_compass.h>
#include <compression/test_qry_zip.h>
#include <compression/test_qry_lzw.h>

#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>

#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

#include <compression/test_qry_spiht.h>
#include <query/op/huffman/qry_huffman_encode_decode.h>

#include <exp/experimentsInfo.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
bool equalTest(pArray arr, int64_t value)
{
	BOOST_LOG_TRIVIAL(debug) << "##############################";
	BOOST_LOG_TRIVIAL(debug) << "Value Equal Test";
	BOOST_LOG_TRIVIAL(debug) << "##############################";

	size_t numCells = 0;

	for (auto attrDesc : *arr->getDesc()->attrDescs_)
	{
		auto cit = arr->getChunkIterator();
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto bit = (**cit)->getBlockIterator();

				while (!bit->isEnd())
				{
					if (bit->isExist())
					{
						auto iit = (**bit)->getItemIterator();

						while (!iit->isEnd())
						{
							if (iit->isExist())
							{
								EXPECT_EQ((**iit).get<value_type>(), static_cast<value_type>(value));
								++numCells;
							}
							++(*iit);
						}
					}
					++(*bit);
				}
			}
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(info) << "[Validation] num values: " << numCells << std::endl;
	return true;
}

// ##################################################
// # Test Body for Sequencial Random Naive/Index Filter
//
// Load: naive
// Zip: naive
// Spiht: naive
// SE: index
// COMPASS: index

template <typename value_type>
pArray test_body_seq_load_random_naive_filter(_pFuncGetSourceArray_,
											  _pFuncGetSourceArrayDesc_,
											  size_t numTests,
											  bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_load_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_zip_random_naive_filter(_pFuncGetSourceArray_,
											 _pFuncGetSourceArrayDesc_,
											 size_t numTests,
											 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_zip_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												   v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_lzw_random_naive_filter(_pFuncGetSourceArray_,
											 _pFuncGetSourceArrayDesc_,
											 size_t numTests,
											 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_lzw_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												   v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_huffman_random_naive_filter(_pFuncGetSourceArray_,
												 _pFuncGetSourceArrayDesc_,
												 size_t numTests,
												 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_huffman_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													   v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_spiht_random_naive_filter(_pFuncGetSourceArray_,
											   _pFuncGetSourceArrayDesc_,
											   eleDefault wtLevel,
											   size_t numTests,
											   bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_spiht_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													 wtLevel, v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_compass_random_naive_filter(_pFuncGetSourceArray_,
												 _pFuncGetSourceArrayDesc_,
												 eleDefault numBins,
												 size_t numTests,
												 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_compass_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													   numBins, v, saveArray, validation, printFlag, i);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_compass_random_index_filter(_pFuncGetSourceArray_,
												 _pFuncGetSourceArrayDesc_,
												 eleDefault numBins,
												 size_t numTests,
												 bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Build Compass Index
	// TODO:: Load Compass Index from a disk
	// Do not print in saveArray
	test_qry_ind_compass_index_build<value_type>(getSourceArrayIfEmpty,
												 getSourceArrayDesc,
												 numBins);
	//////////////////////////////

	//////////////////////////////
	// 03. Save Source Array
	if (saveArray)
	{
		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);		// Do not print in saveArray
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);			// Do not print in saveArray
	}

	//////////////////////////////
	// 04. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_compass_index_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
													   numBins, v, false, validation, printFlag, i);	// Set saveArray = false
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_se_random_naive_filter(_pFuncGetSourceArray_,
											_pFuncGetSourceArrayDesc_,
											eleDefault wtLevel, eleDefault mmtLevel,
											size_t numTests,
											bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);					// Do not print in saveArray
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);	// Do not print in saveArray
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Build MMT
	//test_body_mmt_build<value_type>(getSourceArrayIfEmpty, mmtLevel, false);	// MMT is used to reconstruct the array
	test_body_mmt_load<value_type>(getSourceArrayDesc, validation, false);		// MMT is used to reconstruct the array
	//////////////////////////////

	//////////////////////////////
	// 04. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_se_naive_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												  wtLevel, mmtLevel, v, false, validation, printFlag, i);	// Set saveArray = false
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

template <typename value_type>
pArray test_body_seq_se_random_index_filter(_pFuncGetSourceArray_,
											_pFuncGetSourceArrayDesc_,
											eleDefault wtLevel, eleDefault mmtLevel,
											size_t numTests,
											bool saveArray = false, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Set Seed For Random Value 
	srand(filterValueSeed);
	value_type v = 0;
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);					// Do not print in saveArray
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);	// Do not print in saveArray
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Build MMT
	//test_body_mmt_build<value_type>(getSourceArrayIfEmpty, mmtLevel, false);
	test_body_mmt_load<value_type>(getSourceArrayDesc, validation, false);
	//////////////////////////////

	//////////////////////////////
	// 04. Execute Testcases
	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		test_body_seq_se_index_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
												  wtLevel, mmtLevel, v, false, validation, printFlag, i);		// Set saveArray = false
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
	//////////////////////////////

	return nullptr;
}

// ##################################################

// ##################################################
// # Test Body for Sequencial Naive/Index Filter
template <typename value_type>
pArray test_body_seq_load_naive_filter(_pFuncGetSourceArray_,
									   _pFuncGetSourceArrayDesc_,
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
	// 02. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_load_naive_filter<value_type>(sourceArr,
																myNaivePredicate,
																saveArray, printFlag,
																experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
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

template <typename value_type>
pArray test_body_seq_spiht_naive_filter(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault wtLevel, int64_t value,
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
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_spiht_naive_filter<value_type>(sourceArr,
																 myPredicate, wtLevel,
																 saveArray, printFlag,
																 experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_se_naive_filter(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault wtLevel, eleDefault mmtLevel,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Se Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_se_naive_filter<value_type>(sourceArr,
															  myPredicate, wtLevel, mmtLevel,
															  saveArray, printFlag,
															  experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################";
		BOOST_LOG_TRIVIAL(debug) << "Validation";
		BOOST_LOG_TRIVIAL(debug) << "##############################";

		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_se_index_filter(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault wtLevel, eleDefault mmtLevel,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Se Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_se_index_filter<value_type>(sourceArr,
															  myPredicate, wtLevel, mmtLevel,
															  saveArray, printFlag,
															  experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################";
		BOOST_LOG_TRIVIAL(debug) << "Validation";
		BOOST_LOG_TRIVIAL(debug) << "##############################";

		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_compass_naive_filter(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
										  eleDefault numBins, int64_t value,
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
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_compass_naive_filter<value_type>(sourceArr,
																   myPredicate, numBins,
																   saveArray, printFlag,
																   experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_compass_index_filter(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
										  eleDefault numBins, int64_t value,
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
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_compass_index_filter<value_type>(sourceArr,
																   myPredicate, numBins,
																   saveArray, printFlag,
																   experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_zip_naive_filter(_pFuncGetSourceArray_,
									  _pFuncGetSourceArrayDesc_,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_zip_naive_filter<value_type>(sourceArr,
															   myPredicate,
															   saveArray, printFlag,
															   experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_naive_filter(_pFuncGetSourceArray_,
									  _pFuncGetSourceArrayDesc_,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_lzw_naive_filter<value_type>(sourceArr,
															   myPredicate,
															   saveArray, printFlag,
															   experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_huffman_naive_filter(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
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
	sourceArr[0]->setId(sourceArr[0]->getId() + huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_huffman_naive_filter<value_type>(sourceArr,
																   myPredicate,
																   saveArray, printFlag,
																   experiments::filter_random::expId, expTrial);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	if (validation)
	{
		//////////////////////////////
		// 03-1. Naive Filter Array
		pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
		auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
																 myNaivePredicate, false);
		//////////////////////////////

		//////////////////////////////
		// 03-2. Filtered Value Test
		equalTest<value_type>(outArr, value);
		//////////////////////////////

		//////////////////////////////
		// 03-3. Filtered Array Test
		compArrary<value_type>(filterOutArr, outArr);
		//////////////////////////////
	}
	//////////////////////////////

	return outArr;
}

// ##################################################

pArray exe_act_ind_naive_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

pArray exe_act_ind_index_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

pTerm getEqualTerm(int64_t value, attributeId attrId = 0);

template <typename value_type>
pArray exe_qbundle_seq_load_naive_filter(_vectorSourceArray_,
										 pPredicate myPredicate,
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
	// 02. Naive Filter Array
	return exe_qry_seq_load_naive_filter<value_type>(sourceArr,
													 myPredicate,
													 printFlag,
													 expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_spiht_naive_filter(_vectorSourceArray_,
										  pPredicate inPredicate,
										  eleDefault wtLevel,
										  bool saveArray = false, bool printFlag = false,
										  size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, false);
	}

	//////////////////////////////
	// 02 Naive Filter Arrary
	return exe_qry_seq_spiht_naive_filter<value_type>(sourceArr,
													  inPredicate,
													  wtLevel,
													  printFlag,
													  expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_se_naive_filter(_vectorSourceArray_,
									   pPredicate inPredicate,
									   eleDefault wtLevel,
									   eleDefault mmtLevel,
									   bool saveArray = false, bool printFlag = false,
									   size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);
	}

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_se_naive_filter<value_type>(sourceArr,
												   inPredicate,
												   wtLevel, mmtLevel,
												   printFlag,
												   expId, expTrial);
	//////////////////////////////
}


template <typename value_type>
pArray exe_qbundle_seq_se_index_filter(_vectorSourceArray_,
									   pPredicate inPredicate,
									   eleDefault wtLevel,
									   eleDefault mmtLevel,
									   bool saveArray = false, bool printFlag = false,
									   size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);
	}

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_se_index_filter<value_type>(sourceArr,
												   inPredicate,
												   wtLevel, mmtLevel,
												   printFlag,
												   expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_compass_naive_filter(_vectorSourceArray_,
											pPredicate inPredicate,
											eleDefault numBins,
											bool saveArray = false, bool printFlag = false,
											size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);
	}

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_compass_naive_filter<value_type>(sourceArr,
														inPredicate,
														numBins,
														printFlag,
														expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_compass_index_filter(_vectorSourceArray_,
											pPredicate inPredicate,
											eleDefault numBins,
											bool saveArray = false, bool printFlag = false,
											size_t expId = 0, size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);
	}

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_compass_index_filter<value_type>(sourceArr,
														inPredicate,
														numBins,
														printFlag,
														expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_zip_naive_filter(_vectorSourceArray_,
										pPredicate inPredicate,
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
	// 02 Naive Filter Arrary
	return exe_qry_seq_zip_naive_filter<value_type>(sourceArr,
													inPredicate,
													printFlag,
													expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_lzw_naive_filter(_vectorSourceArray_,
										pPredicate inPredicate,
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
	// 02 Naive Filter Arrary
	return exe_qry_seq_lzw_naive_filter<value_type>(sourceArr,
													inPredicate,
													printFlag,
													expId, expTrial);
	//////////////////////////////
}

template <typename value_type>
pArray exe_qbundle_seq_huffman_naive_filter(_vectorSourceArray_,
											pPredicate inPredicate,
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
	// 02 Naive Filter Arrary
	return exe_qry_seq_huffman_naive_filter<value_type>(sourceArr,
														inPredicate,
														printFlag,
														expId, expTrial);
	//////////////////////////////
}


template <typename value_type>
pArray exe_qry_ind_naive_filter(_vectorSourceArray_,
								pPredicate inPredicate,
								bool printFlag = false)
{
	auto outArr = exe_act_ind_naive_filter(sourceArr, inPredicate);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_naive_filter(_vectorSourceArray_,
									 pPredicate inPredicate,
									 bool printFlag = false,
									 size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getNaiveFilterPlan(loadPlan, inPredicate, qry);

	auto loadAction = loadPlan->getAction();
	auto outArr = loadAction->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Load Arr" << std::endl;
		outArr->print();
	}

	auto filterAction = filterPlan->getAction();
	outArr = filterAction->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_index_filter(_vectorSourceArray_,
									 pPredicate inPredicate,
									 bool printFlag = false,
									 size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getIndexFilterPlan(loadPlan, inPredicate, qry);

	auto loadAction = loadPlan->getAction();
	auto outArr = loadAction->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Load Arr" << std::endl;
		outArr->print();
	}

	auto filterAction = filterPlan->getAction();
	outArr = filterAction->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_naive_filter(_vectorSourceArray_,
									  pPredicate inPredicate,
									  eleDefault wtLevel,
									  bool printFlag = false,
									  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto spihtPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spihtPlan, wtLevel, qry);
	auto filterPlan = getNaiveFilterPlan(wtDecodePlan, inPredicate, qry);

	auto outArr = spihtPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_naive_filter(_vectorSourceArray_,
								   pPredicate inPredicate,
								   eleDefault wtLevel,
								   eleDefault mmtLevel,
								   bool printFlag = false,
								   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	//auto deltaDecodePlan = getMMTDeltaDecodePlan(seDecompPlan, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto filterPlan = getNaiveFilterPlan(wtDecodePlan, inPredicate, qry);

	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	//outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//if (printFlag)
	//{
	//	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	//	BOOST_LOG_TRIVIAL(debug) << "Delta Decode Arr" << std::endl;
	//	outArr->print();
	//	//outArr->getChunkBitmap()->print();
	//}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId(), 0);

	return outArr;
}


template <typename value_type>
pArray exe_qry_seq_se_index_filter(_vectorSourceArray_,
								   pPredicate inPredicate,
								   eleDefault wtLevel,
								   eleDefault mmtLevel,
								   bool printFlag = false,
								   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	//auto deltaDecodePlan = getMMTDeltaDecodePlan(seDecompPlan, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto filterPlan = getIndexFilterPlan(wtDecodePlan, inPredicate, qry);
	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Decomp Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	//outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//if (printFlag)
	//{
	//	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	//	BOOST_LOG_TRIVIAL(debug) << "Delta Decode Arr" << std::endl;
	//	outArr->print();
	//	//outArr->getChunkBitmap()->print();
	//}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId(), 1);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_naive_filter(_vectorSourceArray_,
										pPredicate inPredicate,
										eleDefault numBins,
										bool printFlag = false,
										size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto cpDecodePlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);
	auto filterPlan = getNaiveFilterPlan(cpDecodePlan, inPredicate, qry);

	auto outArr = cpDecodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Compass Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId(), 0);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_index_filter(_vectorSourceArray_,
										pPredicate inPredicate,
										eleDefault numBins,
										bool printFlag = false,
										size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto cpDecodePlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);
	auto outArr = cpDecodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Compass Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	// Its special case of COMPASS index filter
	// To use index in FilterPlan, the hole array should be reconstructred before the query
	auto filterPlan = getIndexFilterPlan(cpDecodePlan, inPredicate, qry);
	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId(), 1);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_zip_naive_filter(_vectorSourceArray_,
									pPredicate inPredicate,
									bool printFlag = false,
									size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto zipLoadPlan = getZipLoadPlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getNaiveFilterPlan(zipLoadPlan, inPredicate, qry);

	auto outArr = zipLoadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Zip Load Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_lzw_naive_filter(_vectorSourceArray_,
									pPredicate inPredicate,
									bool printFlag = false,
									size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLzwDecodePlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getNaiveFilterPlan(loadPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Lzw Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_huffman_naive_filter(_vectorSourceArray_,
										pPredicate inPredicate,
										bool printFlag = false,
										size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getNaiveFilterPlan(loadPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Huffman Decode Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Filtered Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}
}		// caDummy
}		// msdb
#endif	//_MSDB_TESTFILTER_H_
