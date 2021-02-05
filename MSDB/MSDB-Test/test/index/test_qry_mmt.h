#pragma once
#ifndef _MSDB_TEST_QRY_MMT_H_
#define _MSDB_TEST_QRY_MMT_H_

#include <pch.h>
#include <index/test_action_mmt.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
void print_mmt(pArray arr, attributeId attrId)
{
	auto attrIndex = arrayMgr::instance()->getAttributeIndex(arr->getId(), attrId);
	auto myMMT = std::static_pointer_cast<MinMaxTreeImpl<position_t, value_type>>(attrIndex);
	myMMT->print();
}

template <typename value_type>
pArray test_body_mmt_build(_pFuncGetSourceArray_, eleDefault mmtLevel, bool printFlag = false)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. MMT Build
	auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	//////////////////////////////

	//////////////////////////////
	// TODO::validation
	//////////////////////////////

	return arr_mmt_build;
}

template <typename value_type>
pArray exe_qry_ind_mmt_build(_vectorSourceArray_, eleDefault mmtLevel, bool printFlag = false)
{
	auto outArr = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}
}
}
#endif	// _MSDB_TEST_QRY_MMT_H_
