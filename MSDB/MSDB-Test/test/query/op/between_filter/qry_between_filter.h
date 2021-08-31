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

#include <compression/test_qry_spiht.h>
#include <query/op/huffman/qry_huffman_encode_decode.h>
#include <query/op/adapt_huffman/qry_adapt_huffman_encode_decode.h>
#include <query/op/lzw_huffman/qry_lzw_huffman_encode_decode.h>
#include <query/op/se_huffman/qry_se_huffman_encode_decode.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
static int randomValueLimit = 128;

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

template <typename value_type>
pArray exe_qry_seq_spiht_between_filter(_vectorSourceArray_,
										eleDefault wtLevel,
										coor sp, coor ep,
										pPredicate inPredicate,
										bool printFlag = false,
										size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtPlan = getWaveletDecodePlan(loadPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "SPIHT Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = wtPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
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

template <typename value_type>
pArray exe_qry_seq_compass_naive_between_filter(_vectorSourceArray_,
												eleDefault numBins,
												coor sp, coor ep,
												pPredicate inPredicate,
												bool printFlag = false,
												size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Compass Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Naive Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_index_between_filter(_vectorSourceArray_,
												eleDefault numBins,
												coor sp, coor ep,
												pPredicate inPredicate,
												bool printFlag = false,
												size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getIndexFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Compass Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Index Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_index_between_filter(_vectorSourceArray_,
										   eleDefault wtLevel, eleDefault mmtLevel,
										   coor sp, coor ep,
										   pPredicate inPredicate,
										   bool printFlag = false,
										   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtPlan = getWaveletDecodePlan(loadPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtPlan, sp, ep, qry);
	auto filterPlan = getIndexFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = wtPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Index Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_naive_between_filter(_vectorSourceArray_,
										   eleDefault wtLevel, eleDefault mmtLevel,
										   coor sp, coor ep,
										   pPredicate inPredicate,
										   bool printFlag = false,
										   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtPlan = getWaveletDecodePlan(loadPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = wtPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Naive Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_huffman_index_between_filter(_vectorSourceArray_,
												   eleDefault wtLevel, eleDefault mmtLevel,
												   coor sp, coor ep,
												   pPredicate inPredicate,
												   bool printFlag = false,
												   size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getSeHuffmanDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtPlan = getWaveletDecodePlan(loadPlan, wtLevel, qry);
	auto betweenPlan = getBetweenPlan(wtPlan, sp, ep, qry);
	auto filterPlan = getIndexFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Se Huffman Arr" << std::endl;
		outArr->print();
	}

	outArr = wtPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Wt Decode Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Index Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry, expId, expTrial, sourceArr[0]->getId());

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_lzw_between_filter(_vectorSourceArray_,
									  coor sp, coor ep,
									  pPredicate inPredicate,
									  bool printFlag = false,
									  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLzwDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Lzw Decode Arr" << std::endl;
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

template <typename value_type>
pArray exe_qry_seq_lzw_huffman_between_filter(_vectorSourceArray_,
											  coor sp, coor ep,
											  pPredicate inPredicate,
											  bool printFlag = false,
											  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLzwHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "LZW Huffman Arr" << std::endl;
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

template <typename value_type>
pArray exe_qry_seq_huffman_between_filter(_vectorSourceArray_,
										  coor sp, coor ep,
										  pPredicate inPredicate,
										  bool printFlag = false,
										  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Huffman Decode Arr" << std::endl;
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

template <typename value_type>
pArray exe_qry_seq_adapt_huffman_between_filter(_vectorSourceArray_,
												coor sp, coor ep,
												pPredicate inPredicate,
												bool printFlag = false,
												size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getAdaptHuffmanDecodePlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Adapt Huffman Arr" << std::endl;
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

template <typename value_type>
pArray exe_qry_seq_zip_between_filter(_vectorSourceArray_,
									  coor sp, coor ep,
									  pPredicate inPredicate,
									  bool printFlag = false,
									  size_t expId = 0, size_t expTrial = 0)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getZipLoadPlan(sourceArr[0]->getDesc(), qry);
	auto betweenPlan = getBetweenPlan(loadPlan, sp, ep, qry);
	auto filterPlan = getNaiveFilterPlan(betweenPlan, inPredicate, qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Zip Arr" << std::endl;
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
										 coor sp, coor ep, int64_t value,
										 bool saveArray = false, bool validation = false, bool printFlag = false,
										 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
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

template <typename value_type>
pArray test_body_seq_spiht_between_filter(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
										  eleDefault wtLevel,
										  coor sp, coor ep, int64_t value,
										  bool saveArray = false, bool validation = false, bool printFlag = false,
										  size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_spiht_between_filter<value_type>(sourceArr, wtLevel, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_compass_index_between_filter(_pFuncGetSourceArray_,
												  _pFuncGetSourceArrayDesc_,
												  eleDefault numBins,
												  coor sp, coor ep, int64_t value,
												  bool saveArray = false, bool validation = false, bool printFlag = false,
												  size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_compass_index_between_filter<value_type>(sourceArr, numBins, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_compass_naive_between_filter(_pFuncGetSourceArray_,
												  _pFuncGetSourceArrayDesc_,
												  eleDefault numBins,
												  coor sp, coor ep, int64_t value,
												  bool saveArray = false, bool validation = false, bool printFlag = false,
												  size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_compass_naive_between_filter<value_type>(sourceArr, numBins, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_se_index_between_filter(_pFuncGetSourceArray_,
											 _pFuncGetSourceArrayDesc_,
											 eleDefault wtLevel, eleDefault mmtLevel,
											 coor sp, coor ep, int64_t value,
											 bool saveArray = false, bool validation = false, bool printFlag = false,
											 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_se_index_between_filter<value_type>(sourceArr, wtLevel, mmtLevel,
																  sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_se_naive_between_filter(_pFuncGetSourceArray_,
											 _pFuncGetSourceArrayDesc_,
											 eleDefault wtLevel, eleDefault mmtLevel,
											 coor sp, coor ep, int64_t value,
											 bool saveArray = false, bool validation = false, bool printFlag = false,
											 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
		exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_se_naive_between_filter<value_type>(sourceArr, wtLevel, mmtLevel,
																  sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_se_huffman_index_between_filter(_pFuncGetSourceArray_,
													 _pFuncGetSourceArrayDesc_,
													 eleDefault wtLevel, eleDefault mmtLevel,
													 coor sp, coor ep, int64_t value,
													 bool saveArray = false, bool validation = false, bool printFlag = false,
													 size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
		exe_qry_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_se_huffman_index_between_filter<value_type>(sourceArr, wtLevel, mmtLevel,
																		  sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_lzw_between_filter(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										coor sp, coor ep, int64_t value,
										bool saveArray = false, bool validation = false, bool printFlag = false,
										size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_lzw_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_lzw_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_lzw_huffman_between_filter(_pFuncGetSourceArray_,
												_pFuncGetSourceArrayDesc_,
												coor sp, coor ep, int64_t value,
												bool saveArray = false, bool validation = false, bool printFlag = false,
												size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_lzw_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_lzw_huffman_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_huffman_between_filter(_pFuncGetSourceArray_,
											_pFuncGetSourceArrayDesc_,
											coor sp, coor ep, int64_t value,
											bool saveArray = false, bool validation = false, bool printFlag = false,
											size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_huffman_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_adapt_huffman_between_filter(_pFuncGetSourceArray_,
												  _pFuncGetSourceArrayDesc_,
												  coor sp, coor ep, int64_t value,
												  bool saveArray = false, bool validation = false, bool printFlag = false,
												  size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + adapt_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_adapt_huffman_encode<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_adapt_huffman_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
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

template <typename value_type>
pArray test_body_seq_zip_between_filter(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										coor sp, coor ep, int64_t value,
										bool saveArray = false, bool validation = false, bool printFlag = false,
										size_t expTrial = 0)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	if (saveArray || validation)
		getSourceArrayIfEmpty(sourceArr);
	else
		getSourceArrayDesc(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_zip_save<value_type>(sourceArr, false);
	}

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qry_seq_zip_between_filter<value_type>(sourceArr, sp, ep, myNaivePredicate,
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
			v = (value_type)rand() % randomValueLimit;

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

template <typename value_type>
pArray test_body_seq_zip_random_between_filter(_pFuncGetSourceArray_,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_zip_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
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

template <typename value_type>
pArray test_body_seq_lzw_random_between_filter(_pFuncGetSourceArray_,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_lzw_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
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

template <typename value_type>
pArray test_body_seq_huffman_random_between_filter(_pFuncGetSourceArray_,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_huffman_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
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

template <typename value_type>
pArray test_body_seq_adapt_huffman_random_between_filter(_pFuncGetSourceArray_,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_adapt_huffman_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
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

template <typename value_type>
pArray test_body_seq_lzw_huffman_random_between_filter(_pFuncGetSourceArray_,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_lzw_huffman_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
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

template <typename value_type>
pArray test_body_seq_spiht_random_between_filter(_pFuncGetSourceArray_,
												 _pFuncGetSourceArrayDesc_,
												 eleDefault wtLevel,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_spiht_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
														   wtLevel,
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

template <typename value_type>
pArray test_body_seq_compass_naive_random_between_filter(_pFuncGetSourceArray_,
														 _pFuncGetSourceArrayDesc_,
														 eleDefault numBins,
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
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_compass_naive_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
																   numBins,
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

template <typename value_type>
pArray test_body_seq_compass_index_random_between_filter(_pFuncGetSourceArray_,
														 _pFuncGetSourceArrayDesc_,
														 eleDefault numBins,
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
	// 02. Save Source Array
	if (saveArray)
	{
		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

		exe_qry_ind_compass_index_build<value_type>(sourceArr, numBins, false);		// Do not print in saveArray
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, false);			// Do not print in saveArray
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Build Compass Index
	// TODO:: Load Compass Index from a disk
	// Do not print in saveArray
	if (!saveArray)
	{
		test_qry_ind_compass_index_build<value_type>(getSourceArrayIfEmpty,
													 getSourceArrayDesc,
													 numBins);
	}
	//////////////////////////////

	//////////////////////////////
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_compass_index_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
																   numBins,
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

template <typename value_type>
pArray test_body_seq_se_naive_random_between_filter(_pFuncGetSourceArray_,
													_pFuncGetSourceArrayDesc_,
													eleDefault wtLevel, eleDefault mmtLevel,
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
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_se_naive_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
															  wtLevel, mmtLevel,
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

template <typename value_type>
pArray test_body_seq_se_index_random_between_filter(_pFuncGetSourceArray_,
													_pFuncGetSourceArrayDesc_,
													eleDefault wtLevel, eleDefault mmtLevel,
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
	// 02. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_se_index_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
															  wtLevel, mmtLevel,
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

template <typename value_type>
pArray test_body_seq_se_huffman_index_random_between_filter(_pFuncGetSourceArray_,
															_pFuncGetSourceArrayDesc_,
															eleDefault wtLevel, eleDefault mmtLevel,
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
	// 02. Build MMT
	{
		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);

		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);					// Do not print in saveArray

		//test_body_mmt_build<value_type>(getSourceArrayIfEmpty, mmtLevel, false);
		//test_body_mmt_load<value_type>(getSourceArrayDesc, validation, false);	// has SE HUFFMAN ARRAY ID ERROR
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Save Source Array
	if (saveArray)
	{

		std::vector<pArray> sourceArr;
		getSourceArrayIfEmpty(sourceArr);
		sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);

		exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);					// Do not print in saveArray
		exe_qry_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, false);	// Do not print in saveArray
		
	}
	//////////////////////////////

	//////////////////////////////
	// 04. Execute Testcases
	size_t j = 0;
	for (auto selectivity : selectivities)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			coorRange qRange = getRandomRange(dimX, dimY, selectivity);
			v = (value_type)rand() % randomValueLimit;

			BOOST_LOG_TRIVIAL(info) << "##################################################";
			BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
			BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
			test_body_seq_se_huffman_index_between_filter<value_type>(getSourceArrayIfEmpty, getSourceArrayDesc,
																	  wtLevel, mmtLevel,
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
}		// core
}		// msdb
#endif	// _MSDB_TEST_QRY_BETWEEN_INDEX_H_