#pragma once
#include <pch.h>

#include <op/save/save_plan.h>
#include <op/save/save_action.h>
#include <op/load/load_plan.h>
#include <op/load/load_action.h>

namespace msdb
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

namespace data2D_sc4x4
{
void load_test(pArray arr);
}	// data2D_sc4x4
}	// caDummy
}	// msdb