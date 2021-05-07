#pragma once
#ifndef _MSDB_TEST_QRY_HUFFMAN_H_
#define _MSDB_TEST_QRY_HUFFMAN_H_

#include <pch.h>
#include <dummy/opDummy.h>
#include <util/timer.h>
#include "act_huffman_encode_decode.h"

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray exe_qry_ind_huffman_encode(_vectorSourceArray_,
								  bool printFlag = false)
{
	auto outArr = exe_act_ind_huffman_encode(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Huffman Encode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_huffman_decode(_vectorSourceArray_,
								  bool printFlag = false)
{
	auto outArr = exe_act_ind_huffman_decode(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Huffman Decode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_huffman_encode(_pFuncGetSourceArray_,
								   bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + huffman_array_id);
	//////////////////////////////

	auto outArr = exe_qry_ind_huffman_encode<value_type>(sourceArr, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_huffman_decode(_pFuncGetSourceArrayDesc_,
								   bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + huffman_array_id);
	//////////////////////////////

	auto outArr = exe_qry_ind_huffman_decode<value_type>(sourceArrDesc, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_body_seq_huffman_encode_decode(_pFuncGetSourceArray_,
										   _pFuncGetSourceArrayDesc_,
										   bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	sourceArr[0]->setId(sourceArr[0]->getId() + huffman_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + huffman_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Array
	exe_qry_ind_huffman_encode<value_type>(sourceArr, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Array
	auto outArr = exe_qry_ind_huffman_decode<value_type>(sourceArrDesc, printFlag);
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
}	// caDummy
}		// msdb
#endif	// _MSDB_TEST_QRY_HUFFMAN_H_