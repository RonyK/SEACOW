#pragma once
#ifndef _MSDB_TEST_QRY_SE_HUFFMAN_H_
#define _MSDB_TEST_QRY_SE_HUFFMAN_H_

#include <pch.h>
#include <dummy/opDummy.h>
#include <util/timer.h>
#include <index/test_qry_mmt.h>
#include <compression/test_action_compression.h>
#include "act_se_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
template <typename value_type>
pArray exe_qry_ind_se_huffman_encode(_vectorSourceArray_,
									 eleDefault wtLevel, eleDefault mmtLevel,
									 bool printFlag = false)
{
	auto outArr = exe_act_ind_wavelet_encode(sourceArr, wtLevel);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################";
		BOOST_LOG_TRIVIAL(debug) << "Wavelet Encode Arr";
		outArr->print();
	}

	//outArr = exe_act_ind_mmt_delta_encode(std::vector<pArray>({ outArr }));
	//if (printFlag)
	//{
	//	BOOST_LOG_TRIVIAL(debug) << "##############################";
	//	BOOST_LOG_TRIVIAL(debug) << "MMT Delta Arr";
	//	outArr->print();
	//}

	outArr = exe_act_ind_se_huffman_encode(std::vector<pArray>({ outArr }));
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "SE Huffman Encode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_se_huffman_decode(_vectorSourceArray_,
									 eleDefault wtLevel, eleDefault mmtLevel,
									 bool printFlag = false)
{
	auto outArr = exe_act_ind_se_huffman_decode(sourceArr, wtLevel);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "SE Huffman Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = exe_act_ind_wavelet_decode(std::vector<pArray>({ outArr }), wtLevel);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################";
		BOOST_LOG_TRIVIAL(debug) << "Wavelet Decode Arr";
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_huffman_decode(_vectorSourceArray_,
									 eleDefault wtLevel, eleDefault mmtLevel,
									 bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();
	auto decodePlan = getSeHuffmanDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(decodePlan, wtLevel, qry);

	auto outArr = decodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "SE Huffman Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################";
		BOOST_LOG_TRIVIAL(debug) << "Wavelet Decode Arr";
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qbundle_ind_se_huffman_encode(_vectorSourceArray_,
										 eleDefault wtLevel, eleDefault mmtLevel,
										 bool printFlag = false)
{
	//////////////////////////////
	// 01. Build MMT
	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);

	//////////////////////////////
	// 02. Save SE Array
	return exe_qry_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
}

template <typename value_type>
pArray test_qry_ind_se_huffman_encode(_pFuncGetSourceArray_,
									  eleDefault wtLevel, eleDefault mmtLevel,
									  bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	auto outArr = exe_qbundle_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_se_huffman_decode(_pFuncGetSourceArrayDesc_,
									  eleDefault wtLevel, eleDefault mmtLevel,
									  bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	auto outArr = exe_qry_ind_se_huffman_decode<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_seq_se_huffman_decode(_pFuncGetSourceArrayDesc_,
									  eleDefault wtLevel, eleDefault mmtLevel,
									  bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	auto outArr = exe_qry_seq_se_huffman_decode<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_body_ind_se_huffman_encode_decode(_pFuncGetSourceArray_,
											  _pFuncGetSourceArrayDesc_,
											  eleDefault wtLevel, eleDefault mmtLevel,
											  bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Array
	exe_qbundle_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Array
	auto outArr = exe_qry_ind_se_huffman_decode<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Evaluation
	if (validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_se_huffman_encode_decode(_pFuncGetSourceArray_,
											  _pFuncGetSourceArrayDesc_,
											  eleDefault wtLevel, eleDefault mmtLevel,
											  bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_huffman_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Array
	exe_qbundle_ind_se_huffman_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Array
	auto outArr = exe_qry_seq_se_huffman_decode<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);
	//////////////////////////////

	////////////////////////////////
	//// 04. Evaluation
	if (validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
	}
	////////////////////////////////

	//return outArr;
	return nullptr;
}
}		// caDummy
}		// core
}		// msdb
#endif	// _MSDB_TEST_QRY_SE_HUFFMAN_H_