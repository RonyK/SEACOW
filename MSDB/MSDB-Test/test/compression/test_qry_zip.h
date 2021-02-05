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
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto outArr = exe_qry_ind_zip_save<value_type>(sourceArr, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_zip_save_load(_pFuncGetSourceArray_,
								  _pFuncGetSourceArrayDesc_,
								  bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + compass_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + compass_array_id);

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
		BOOST_LOG_TRIVIAL(debug) << "Zip Encode Arr" << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "Zip Decode Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_QRY_ZIP_