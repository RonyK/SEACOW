#pragma once
#include <pch.h>
#include <index/test_action_mmt.h>

#include <array/arrayMgr.h>
#include <system/storageMgr.h>

#include <index/mmt.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>
#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>
#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>

namespace msdb
{
namespace caDummy
{
pArray exe_act_ind_mmt_build(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	// Execute mmt build action
	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

pArray exe_act_ind_mmt_save(std::vector<pArray> sourceArr)
{
	std::shared_ptr<mmt_save_plan> mmtPlan;
	std::shared_ptr<mmt_save_action> mmtAction;
	pQuery mmtQuery;
	getMmtSave(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

pArray exe_act_ind_mmt_load(std::vector<pArray> sourceArr)
{
	std::shared_ptr<mmt_load_plan> mmtPlan;
	std::shared_ptr<mmt_load_action> mmtAction;
	pQuery mmtQuery;
	getMmtLoad(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);
	auto attrIndex = arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), 0);

	return afterArray;
}

pArray exe_act_ind_mmt_delta_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<mmt_delta_encode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_encode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaEncode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

pArray exe_act_ind_mmt_delta_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<mmt_delta_decode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_decode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaDecode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

namespace data2D_sc4x4
{
void mmt_build_test(pArray afterArray)
{
	// Result nextWork
	for (auto attrDesc : *afterArray->getDesc()->attrDescs_)
	{
		auto attrIndex = arrayMgr::instance()->getAttributeIndex(afterArray->getId(), attrDesc->id_);
		EXPECT_TRUE(attrIndex != nullptr);

		std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(attrIndex);
		auto nodes = mmtIndex->getNodes();
		size_t level = mmtIndex->getMaxLevel();
		EXPECT_EQ(nodes.size(), level + 1);

		value_type exMMT_min[2][2][2];
		value_type exMMT_max[2][2][2];
		getExMMTBuilded(exMMT_min, exMMT_max, 2 * 2 * 2);

		for (size_t l = 0; l <= level; l++)
		{
			auto levelNodes = nodes[l];
			auto nit = mmtIndex->getNodeIterator(l);
			//MinMaxTreeImpl<dim_type, value_type>::nodeItr nit(2, mmtIndex->getNodeSpace(l).data());
			for (int y = 0; y < chunkNums[0] / pow(2, l); ++y)
			{
				for (int x = 0; x < chunkNums[1] / pow(2, l); ++x)
				{
					coor blockCoor({ y, x });
					nit.moveTo(blockCoor);
					auto node = levelNodes.data()[nit.seqPos()];

					//std::cout << "[" << x << ", " << y << "] " << static_cast<int>(node->getMin<value_type>()) << " ~ " << static_cast<int>(node->getMax<value_type>()) << std::endl;

					EXPECT_EQ(exMMT_min[l][y][x], node->getMin<value_type>());
					EXPECT_EQ(exMMT_max[l][y][x], node->getMax<value_type>());
				}
			}
		}
	}
}

void mmt_delta_encode_test(std::shared_ptr<mmt_delta_encode_array> arr)
{
	auto arrId = arr->getId();

	for (auto attr : *arr->getDesc()->attrDescs_)
	{
		value_type expected[dataLength];
		getExDelta(expected, dataLength);

		auto cit = arr->getChunkIterator();
		size_t c = 0, numCheckedItems = 0;

		while (!cit->isEnd())
		{
			EXPECT_TRUE(false);
			//auto iit = (**cit)->getItemIterator();
			//for (size_t i = 0; i < iit->getCapacity(); ++i)
			//{
			//	//std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
			//	EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
			//	++(*iit);
			//	++numCheckedItems;
			//}
			//++c;
			++(*cit);
		}

		EXPECT_EQ(c, 4);		// chunk num
		EXPECT_EQ(numCheckedItems, dataLength);
	}
}

std::shared_ptr<mmt_delta_encode_array> get_mmt_delta_encode_array()
{
	auto arr_1 = exe_act_ind_mmt_build(std::vector<pArray>(), mmtLevel);
	auto arr_2 = exe_act_ind_mmt_delta_encode(std::vector<pArray>({ arr_1 }));

	return std::static_pointer_cast<mmt_delta_encode_array>(arr_2);
}

void mmt_delta_decode_test(std::shared_ptr<mmt_delta_decode_array> arr)
{
	auto arrId = arr->getId();

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
			//	//std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
			//	EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
			//	++(*iit);
			//}
			//++c;
			++(*cit);
		}
	}
}
}	// data2D_sc4x4
}	// caDummy
}	// msdb