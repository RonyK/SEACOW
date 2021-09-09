#pragma once
#include <pch_test.h>
#include <compression/test_action_compression.h>
#include <dummy/opDummy.h>
#include <util/timer.h>

#ifndef _MSDB_TEST_QRY_LZW_H_
#define _MSDB_TEST_QRY_LZW_H_

namespace msdb
{
namespace core
{
namespace caDummy
{
template <typename value_type>
pArray exe_qry_ind_lzw_encode(_vectorSourceArray_,
							  bool printFlag = false)
{
	auto outArr = exe_act_ind_lzw_encode(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Lzw Save Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_lzw_decode(_vectorSourceArray_,
							  bool printFlag = false)
{
	auto outArr = exe_act_ind_lzw_decode(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Lzw Load Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_lzw_decode(_vectorSourceArray_,
							  bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();
	auto loadPlan = getLzwDecodePlan(sourceArr[0]->getDesc(), qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Lzw Load Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_lzw_encode(_pFuncGetSourceArray_,
							   _pFuncGetSourceArrayDesc_,
							   bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);

	auto outArr = exe_qry_ind_lzw_encode<value_type>(sourceArr, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_lzw_decode(_pFuncGetSourceArray_,
							   _pFuncGetSourceArrayDesc_,
							   bool printFlag = false)
{
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + lzw_array_id);

	auto outArr = exe_qry_ind_lzw_decode<value_type>(sourceArrDesc, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_seq_lzw_decode(_pFuncGetSourceArray_,
							   _pFuncGetSourceArrayDesc_,
							   bool printFlag = false)
{
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + lzw_array_id);

	auto outArr = exe_qry_seq_lzw_decode<value_type>(sourceArrDesc, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_decode(_pFuncGetSourceArray_,
								_pFuncGetSourceArrayDesc_,
								bool printFlag = false, bool validation = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + lzw_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Load Array
	auto outArr = exe_qry_seq_lzw_decode<value_type>(sourceArrDesc, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Evaluation
	if (validation)
	{
		auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
		sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);

		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_lzw_encode_decode(_pFuncGetSourceArray_,
									   _pFuncGetSourceArrayDesc_,
									   bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	sourceArr[0]->setId(sourceArr[0]->getId() + lzw_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + lzw_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Array
	exe_qry_ind_lzw_encode<value_type>(sourceArr, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Array
	auto outArr = exe_qry_seq_lzw_decode<value_type>(sourceArrDesc, printFlag);
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
}		// caDummy
}		// core
}		// msdb
#endif	// _MSDB_TEST_QRY_LZW_H_