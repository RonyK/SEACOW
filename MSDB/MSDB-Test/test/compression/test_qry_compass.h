#pragma once

#include <pch.h>
#include <compression/test_action_compression.h>
#include <dummy/opDummy.h>
#include <util/timer.h>

#ifndef _MSDB_TEST_QRY_COMPASS_
#define _MSDB_TEST_QRY_COMPASS_

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_qry_ind_compass_encode(_pFuncGetSourceArray_,
								   _pFuncGetSourceArrayDesc_,
								   eleDefault numBin, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + compass_array_id);

	auto outArr = exe_qry_ind_compass_encode<value_type>(sourceArr, numBin, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_compass_encode_decode(_pFuncGetSourceArray_,
										  _pFuncGetSourceArrayDesc_,
										  eleDefault numBin, bool printFlag = false, bool validation = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + compass_array_id);

	try
	{
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBin, printFlag);
	} catch (...)
	{
		std::cout << "Exception" << std::endl;
	}
	try
	{
		auto outArr = exe_qry_ind_compass_decode<value_type>(sourceArrDesc, numBin, printFlag);

		if (validation)
		{
			compArrary<value_type>(sourceArr[0], outArr);
		}

		return outArr;
	} catch (...)
	{
		std::cout << "Exception" << std::endl;
	}

	return nullptr;
}

template <typename value_type>
pArray test_qry_ind_compass_index_build(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault numBin, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + compass_array_id);

	auto outArr = exe_qry_ind_compass_index_build<value_type>(sourceArr, numBin, printFlag);

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_compass_encode(_vectorSourceArray_,
								  eleDefault numBins,
								  bool printFlag = false)
{
	auto outArr = exe_act_ind_compass_encode(sourceArr, numBins);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "COMPASS Encode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_encode(_vectorSourceArray_,
								  eleDefault numBins,
								  bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto compassEncodePlan = getCompassEncodePlan(sourceArr[0]->getDesc(), numBins, qry);

	auto outArr = compassEncodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "COMPASS Encode Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_compass_decode(_vectorSourceArray_,
								  eleDefault numBins,
								  bool printFlag = false)
{
	auto outArr = exe_act_ind_compass_decode(sourceArr, numBins);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "COMPASS Decode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_compass_decode(_vectorSourceArray_,
								  eleDefault numBins,
								  bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto compassDecodePlan = getCompassDecodePlan(sourceArr[0]->getDesc(), numBins, qry);

	auto outArr = compassDecodePlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "COMPASS Decode Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_compass_index_build(_vectorSourceArray_,
									   eleDefault numBins,
									   bool printFlag = false)
{
	auto outArr = exe_act_ind_compass_index_build(sourceArr, numBins);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "COMPASS Index Build Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray test_qbundle_seq_compass_encode(_vectorSourceArray_,
									   eleDefault numBins,
									   bool printFlag = false)
{
	//////////////////////////////
	// 01. Encode
	return exe_qry_seq_compass_encode<value_type>(sourceArr, numBins, printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray test_qbundle_seq_compass_decode(_vectorSourceArray_,
									   eleDefault numBins,
									   bool saveArray = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	if (saveArray)
	{
		exe_qry_ind_compass_encode<value_type>(sourceArr, numBins, printFlag);
	}

	//////////////////////////////
	// 02. Decode
	auto outArr = exe_qry_seq_compass_decode<value_type>(sourceArr, numBins, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Validation
	//
	//////////////////////////////
}

template <typename value_type>
pArray test_qbundle_seq_compass_encode_decode(_vectorSourceArray_,
											  eleDefault numBins,
											  bool printFlag = false)
{
	//////////////////////////////
	// 01. Decode
	test_qbundle_seq_compass_encode<value_type>(sourceArr, numBins, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 02. Decode
	return test_qbundle_seq_compass_decode<value_type>(sourceArr, numBins, printFlag);
	//////////////////////////////
}

template <typename value_type>
pArray test_body_seq_compass_encode_decode(_pFuncGetSourceArray_,
										   _pFuncGetSourceArrayDesc_,
										   eleDefault numBin,
										   bool printFlag = false, bool validation = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + compass_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Array
	exe_qry_ind_compass_encode<value_type>(sourceArr, numBin, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Decompression Array
	auto outArr = exe_qry_seq_compass_decode<value_type>(sourceArrDesc, numBin, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 04. Evaluation
	if(validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////

	return outArr;
}
}		// caDummy	
}		// msdb
#endif // !_MSDB_TEST_QRY_COMPASS_
