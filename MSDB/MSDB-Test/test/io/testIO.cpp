#include <pch.h>
#include <array/memArray.h>
#include <io/testIO.h>

namespace msdb
{
namespace caDummy
{
void getSave(pArrayDesc sourceArrDesc,
			 std::shared_ptr<save_plan>& plan,
			 std::shared_ptr<save_action>& action,
			 pQuery& qry)
{
	pQuery saveQuery = std::make_shared<query>();
	auto savePlan = std::make_shared<save_plan>();
	auto saveAction = std::make_shared<save_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<save_pset>(params);

	savePlan->setParamSet(pSet);
	saveAction->setArrayDesc(savePlan->inferSchema());
	saveAction->setParams(params);

	plan = savePlan;
	action = saveAction;
	qry = saveQuery;
}

void getLoad(pArrayDesc sourceArrDesc,
			 std::shared_ptr<load_plan>& plan,
			 std::shared_ptr<load_action>& action,
			 pQuery& qry)
{
	pQuery loadQuery = std::make_shared<query>();
	auto loadPlan = std::make_shared<load_plan>();
	auto loadAction = std::make_shared<load_action>();
	parameters params = {
		std::make_shared<opParamArray>(sourceArrDesc)
	};
	auto pSet = std::make_shared<load_pset>(params);

	loadPlan->setParamSet(pSet);
	loadAction->setArrayDesc(loadPlan->inferSchema());
	loadAction->setParams(params);

	plan = loadPlan;
	action = loadAction;
	qry = loadQuery;
}

pArray save(std::vector<pArray> sourceArr,
			void (*getArray)(std::vector<pArray>& sourceArr))
{
	getArray(sourceArr);
	std::shared_ptr<save_plan> savePlan;
	std::shared_ptr<save_action> saveAction;
	pQuery saveQuery;
	getSave(sourceArr[0]->getDesc(), savePlan, saveAction, saveQuery);

	auto afterArray = saveAction->execute(sourceArr, saveQuery);

	return afterArray;
}
pArray load(std::vector<pArray> sourceArr,
			void (*getArray)(std::vector<pArray>& sourceArr))
{
	getArray(sourceArr);
	std::shared_ptr<load_plan> loadPlan;
	std::shared_ptr<load_action> loadAction;
	pQuery loadQuery;
	getLoad(sourceArr[0]->getDesc(), loadPlan, loadAction, loadQuery);

	auto afterArray = loadAction->execute(sourceArr, loadQuery);

	return afterArray;
}

namespace data2D_sc4x4
{
void load_test(pArray arr)
{
	auto arrId = arr->getId();

	size_t items = 0;
	for (auto attr : *arr->getDesc()->attrDescs_)
	{
		value_type expected[dataLength];
		getChunkDummy(expected, dataLength);

		auto cit = arr->getChunkIterator();
		size_t c = 0;

		while (!cit->isEnd())
		{
			auto iit = (**cit)->getItemIterator();
			for (size_t i = 0; i < iit->getCapacity(); ++i)
			{
				std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
				EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
				++(*iit);
				++items;
			}
			++c;
			++(*cit);
		}
	}

	EXPECT_EQ(items, dataLength);
}
}	// data2D_sc4x4
}	// caDummy
}	// msdb