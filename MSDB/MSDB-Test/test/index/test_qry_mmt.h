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
pArray exe_qry_ind_mmt_build(_vectorSourceArray_, eleDefault mmtLevel, bool printFlag = false)
{
	auto outArr = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

}
}
#endif	// _MSDB_TEST_QRY_MMT_H_
