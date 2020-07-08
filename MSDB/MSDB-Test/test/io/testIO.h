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
namespace data2D_sc4x4
{
void getSave(pArrayDesc sourceArrDesc,
			 std::shared_ptr<save_plan>& plan,
			 std::shared_ptr<save_action>& action,
			 pQuery& qry);
void getLoad(pArrayDesc sourceArrDesc,
			 std::shared_ptr<load_plan>& plan,
			 std::shared_ptr<load_action>& action,
			 pQuery& qry);

pArray save(std::vector<pArray> sourceArr);
pArray load(std::vector<pArray> sourceArr);
}	// data2D_sc4x4
}	// caDummy
}	// msdb