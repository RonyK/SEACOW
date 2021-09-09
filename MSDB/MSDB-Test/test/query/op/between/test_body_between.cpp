#include <pch_test.h>

#include <query/op/between/test_body_between.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
//coorRange getRandomRange(const position_t dimX, const position_t dimY, const float selectivity)
//{
//	int area = dimX * dimY;
//	float selectedArea = area * selectivity / 100.0;
//
//	int spX = 0, spY = 0;
//	int width = 1, height = 1;
//
//	while (1)
//	{
//		spX = rand() % (size_t)(dimX - dimX * selectivity / 100.0);
//		spY = rand() % (size_t)(dimY - dimY * selectivity / 100.0);
//
//		for(int retry = 0; retry < 5; ++retry)
//		{
//			width = rand() % (dimX - spX);
//			if (width > selectedArea)
//				continue;
//
//			height = selectedArea / width;
//
//			if (width > 0 && height > 0 && (spX + width) < dimX && (spY + height) < dimY)
//				break;
//		}
//
//		if (width > 0 && height > 0 && (spX + width) < dimX && (spY + height) < dimY)
//			break;
//	}
//
//	int epX = spX + width;
//	int epY = spY + height;
//	int cal = (epX - spX) * (epY - spY);
//
//	coor sp = { spY, spX };
//	coor ep = { spY + height, spX + width };
//
//	return coorRange(sp, ep);
//}

coorRange getRandomRange(const position_t dimX, const position_t dimY, const float selectivity)
{
	int area = dimX * dimY;
	float selectedArea = area * selectivity / 100.0;

	int width = sqrt(selectivity / 100.0) * dimX, height = sqrt(selectivity / 100.0) * dimY;
	int spX = rand() % (size_t)(dimX - width), spY = rand() % (size_t)(dimY - height);

	int epX = spX + width;
	int epY = spY + height;
	int cal = (epX - spX) * (epY - spY);

	coor sp = { spY, spX };
	coor ep = { spY + height, spX + width };

	return coorRange(sp, ep);
}

void getOpBetween(pArrayDesc sourceArrDesc, std::shared_ptr<between_plan>& plan, std::shared_ptr<between_action>& action, pQuery& qry, coor sp, coor ep)
{
	parameters params = {
	std::make_shared<opParamArray>(sourceArrDesc),
	std::make_shared<opParamCoor>(std::make_shared<coor>(sp)),
	std::make_shared<opParamCoor>(std::make_shared<coor>(ep))
	};

	plan = std::make_shared<between_plan>();
	plan->setParamSet(std::make_shared<between_array_pset>(params));
	action = std::static_pointer_cast<between_action>(plan->getAction());
	qry = std::make_shared<query>();
}

pArray exe_act_ind_raw_between(std::vector<pArray> sourceArr, coor sp, coor ep)
{
	std::shared_ptr<between_plan> myPlan;
	std::shared_ptr<between_action> myAction;
	pQuery myQuery;
	getOpBetween(sourceArr[0]->getDesc(), myPlan, myAction, myQuery, sp, ep);

	auto outArr = myAction->execute(sourceArr, myQuery);

	return outArr;
}

}		// caDummy
}		// core
}		// msdb