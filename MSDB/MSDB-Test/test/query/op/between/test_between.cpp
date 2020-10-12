#include <pch.h>
#include <op/between/between_plan.h>
#include <op/between/between_action.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_between, between_star1024x1024)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);

	

	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	parameters params = {
		std::make_shared<opParamArray>(sourceArr[0]->getDesc()),
		std::make_shared<opParamCoor>(std::make_shared<coor>(sp)),
		std::make_shared<opParamCoor>(std::make_shared<coor>(ep))
	};

	auto betweenQuery = std::make_shared<query>();
	auto betweenPlan = std::make_shared<between_plan>();
	betweenPlan->setParamSet(std::make_shared<between_array_pset>(params));
	auto betweenAction = std::static_pointer_cast<between_action>(betweenPlan->getAction());

	auto outArr = betweenAction->execute(sourceArr, betweenQuery);
	auto it = sourceArr[0]->getChunkIterator();
	++(*it);
	(**it)->print();
	outArr->print();
}
}
}
}