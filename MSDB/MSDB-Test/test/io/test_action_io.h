#pragma once
#include <pch.h>

#include <op/save/save_plan.h>
#include <op/save/save_action.h>
#include <op/load/load_plan.h>
#include <op/load/load_action.h>

namespace msdb
{ 
namespace core
{
namespace caDummy
{
void getSave(pArrayDesc sourceArrDesc,
			 std::shared_ptr<save_plan>& plan,
			 std::shared_ptr<save_action>& action,
			 pQuery& qry);
void getLoad(pArrayDesc sourceArrDesc,
			 std::shared_ptr<load_plan>& plan,
			 std::shared_ptr<load_action>& action,
			 pQuery& qry);

pArray exe_act_ind_save(std::vector<pArray> sourceArr, pQuery qry = nullptr);
pArray exe_act_ind_load(std::vector<pArray> sourceArr, pQuery qry = nullptr);

template <typename value_type>
pArray exe_qry_ind_save(_vectorSourceArray_, bool printFlag = false)
{
	auto outArr = exe_act_ind_save(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Save Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_ind_load(_vectorSourceArray_, bool printFlag = false)
{
	auto outArr = exe_act_ind_load(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Load Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load(_vectorSourceArray_, bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);

	auto outArr = loadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(info) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Load Arr" << std::endl;
		outArr->print();
		//outArr->getChunkBitmap()->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
void test_body_ind_save_load(_pFuncGetSourceArray_, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Arr
	exe_qry_ind_save<value_type>(sourceArr, false);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Arr
	auto outArr = exe_qry_ind_load<value_type>(sourceArr, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 04. Validation
	if(validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////
}

template <typename value_type>
void test_body_seq_save_load(_pFuncGetSourceArray_, bool validation = false, bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	//////////////////////////////

	//////////////////////////////
	// 02. Save Source Arr
	exe_qry_ind_save<value_type>(sourceArr, false);
	//////////////////////////////

	//////////////////////////////
	// 03. Load Arr
	auto outArr = exe_qry_seq_load<value_type>(sourceArr, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 04. Validation
	if (validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
	}
	//////////////////////////////
}

namespace data2D_sc4x4
{
void load_test(pArray arr);
}	// data2D_sc4x4
}	// caDummy
}	// core
}	// msdb