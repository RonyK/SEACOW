#include <pch.h>
#include <array/memArray.h>
#include <io/test_action_io.h>

namespace msdb
{
namespace caDummy
{
pArray exe_act_ind_save(std::vector<pArray> sourceArr, pQuery qry)
{
	std::shared_ptr<save_plan> savePlan;
	std::shared_ptr<save_action> saveAction;
	getSave(sourceArr[0]->getDesc(), savePlan, saveAction, qry);

	auto afterArray = saveAction->execute(sourceArr, qry);

	return afterArray;
}
pArray exe_act_ind_load(std::vector<pArray> sourceArr, pQuery qry)
{
	std::shared_ptr<load_plan> loadPlan;
	std::shared_ptr<load_action> loadAction;
	getLoad(sourceArr[0]->getDesc(), loadPlan, loadAction, qry);

	auto afterArray = loadAction->execute(sourceArr, qry);

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
			EXPECT_TRUE(false);
			//auto iit = (**cit)->getItemIterator();
			//for (size_t i = 0; i < iit->getCapacity(); ++i)
			//{
			//	std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
			//	EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
			//	++(*iit);
			//	++items;
			//}
			//++c;
			++(*cit);
		}
	}

	EXPECT_EQ(items, dataLength);
}
}	// data2D_sc4x4
}	// caDummy
}	// msdb