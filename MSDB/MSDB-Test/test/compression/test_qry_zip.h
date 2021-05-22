#pragma once

#include <pch.h>
#include <compression/test_action_compression.h>
#include <dummy/opDummy.h>
#include <util/timer.h>

#ifndef _MSDB_TEST_QRY_ZIP_
#define _MSDB_TEST_QRY_ZIP_

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_qry_ind_zip_save(_pFuncGetSourceArray_,
							 _pFuncGetSourceArrayDesc_,
							 bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);

	auto outArr = exe_qry_ind_zip_save<value_type>(sourceArr, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_zip_save_load(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + zip_array_id);

	exe_qry_ind_zip_save<value_type>(sourceArr, printFlag);
	auto outArr = exe_qry_ind_zip_load<value_type>(sourceArrDesc, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_zip_save(_vectorSourceArray_,
							bool printFlag = false)
{
	auto outArr = exe_act_ind_zip_save(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Zip Save Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_zip_load(_vectorSourceArray_,
							bool printFlag = false)
{
	auto outArr = exe_act_ind_zip_load(sourceArr);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Zip Load Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_zip_load(_vectorSourceArray_,
							bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto zipLoadPlan = getZipLoadPlan(sourceArr[0]->getDesc(), qry);

	auto outArr = zipLoadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Zip Load Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template<typename value_type>
pArray test_body_seq_zip_load(_pFuncGetSourceArray_,
							  _pFuncGetSourceArrayDesc_,
							  bool printFlag = false, bool validation = false)
{
	//////////////////////////////
	// 01. Get Source Array
	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Load Array
	auto outArr = exe_qry_seq_zip_load<value_type>(sourceArrDesc, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Evaluation
	if (validation)
	{
		auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
		sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);

		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////

	return outArr;
}

template<typename value_type>
pArray test_body_seq_zip_save_load(_pFuncGetSourceArray_,
								   _pFuncGetSourceArrayDesc_,
								   bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	//auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	//sourceArr[0]->setId(sourceArr[0]->getId() + zip_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + zip_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Array
	//exe_qry_ind_zip_save<value_type>(sourceArr, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Array
	auto outArr = exe_qry_seq_zip_load<value_type>(sourceArrDesc, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Evaluation
	//if (validation)
	//{
	//	compArrary<value_type>(sourceArr[0], outArr);
	//}
	//////////////////////////////

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_QRY_ZIP_