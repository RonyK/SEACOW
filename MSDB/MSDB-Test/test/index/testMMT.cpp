#pragma once
#include <pch.h>
#include <index/testMMT.h>

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

#include <string>
#include <memory>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
pArray mmt_build(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 1;
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	// Execute mmt build action
	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

void mmt_build_test(pArray afterArray)
{
	// Result check
	for (auto attrDesc : afterArray->getDesc()->attrDescs_)
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
			MinMaxTreeImpl<dim_type, value_type>::nodeItr nit(2, mmtIndex->getNodeSpace(l).data());
			for (int y = 0; y < chunkNums[0] / pow(2, l); ++y)
			{
				for (int x = 0; x < chunkNums[1] / pow(2, l); ++x)
				{
					coor blockCoor({ y, x });
					nit.moveTo(blockCoor);
					auto node = levelNodes.data()[nit.seqPos()];

					std::cout << "[" << x << ", " << y << "] " << static_cast<int>(node->min_) << " ~ " << static_cast<int>(node->max_) << std::endl;

					EXPECT_EQ(exMMT_min[l][y][x], node->min_);
					EXPECT_EQ(exMMT_max[l][y][x], node->max_);
				}
			}
		}
	}
}

pArray mmt_save(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<mmt_save_plan> mmtPlan;
	std::shared_ptr<mmt_save_action> mmtAction;
	pQuery mmtQuery;
	getMmtSave(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	EXPECT_TRUE(std::filesystem::is_regular_file(
		filePath("../storage/array/441/indies/0.msdbindex")));

	return afterArray;
}

pArray mmt_load(std::vector<pArray> sourceArr)
{

	EXPECT_THROW(arrayMgr::instance()->getAttributeIndex(aid, 0), msdb_exception);
	EXPECT_TRUE(std::filesystem::is_regular_file(
		filePath("../storage/array/441/indies/0.msdbindex")));

	// should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<mmt_load_plan> mmtPlan;
	std::shared_ptr<mmt_load_action> mmtAction;
	pQuery mmtQuery;
	getMmtLoad(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	auto attrIndex = arrayMgr::instance()->getAttributeIndex(aid, 0);
	std::cout << "mmt load" << std::endl;

	return afterArray;
}

pArray mmt_delta_encode(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<mmt_delta_encode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_encode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaEncode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);
	std::cout << "mmt delta encode" << std::endl;

	return afterArray;
}

void mmt_delta_encode_test(std::shared_ptr<mmt_delta_encode_array> arr)
{
	auto arrId = arr->getId();

	for (auto attr : arr->getDesc()->attrDescs_)
	{
		value_type expected[dataLength];
		getExDelta(expected, dataLength);

		auto cit = arr->getChunkIterator();
		size_t c = 0, numCheckedItems = 0;

		while (!cit->isEnd())
		{
			auto iit = (**cit)->getItemIterator();
			for (size_t i = 0; i < iit->getCapacity(); ++i)
			{
				//std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
				EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
				++(*iit);
				++numCheckedItems;
			}
			++c;
			++(*cit);
		}

		EXPECT_EQ(c, 4);		// chunk num
		EXPECT_EQ(numCheckedItems, dataLength);
	}
}

pArray mmt_delta_decode(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	std::shared_ptr<mmt_delta_decode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_decode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaDecode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(std::vector<pArray>({ std::static_pointer_cast<arrayBase>(sourceArr[0]) }), mmtQuery);
	std::cout << "mmt delta decode" << std::endl;

	return afterArray;
}
std::shared_ptr<mmt_delta_encode_array> get_mmt_delta_encode_array()
{
	auto arr_1 = mmt_build();
	auto arr_2 = mmt_delta_encode(std::vector<pArray>({ arr_1 }));

	return std::static_pointer_cast<mmt_delta_encode_array>(arr_2);
}

void mmt_delta_decode_test(std::shared_ptr<mmt_delta_decode_array> arr)
{
	auto arrId = arr->getId();

	for (auto attr : arr->getDesc()->attrDescs_)
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
				//std::cout << "[" << iit->coor()[0] << ", " << iit->coor()[1] << "] " << static_cast<int>((**iit).getChar()) << ", " << static_cast<int>(expected[i]) << std::endl;
				EXPECT_EQ((**iit).getChar(), expected[i + iit->getCapacity() * c]);
				++(*iit);
			}
			++c;
			++(*cit);
		}
	}
}
}	// data2D_sc4x4

namespace data2D_sc8x8
{
pArray mmt_build(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 1;
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	// Execute mmt build action
	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

pArray mmt_save(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<mmt_save_plan> mmtPlan;
	std::shared_ptr<mmt_save_action> mmtAction;
	pQuery mmtQuery;
	getMmtSave(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	EXPECT_TRUE(std::filesystem::is_regular_file(
		filePath("../storage/array/881/indies/0.msdbindex")));

	return afterArray;
}
}

namespace data2D_star1024x1024
{
pArray mmt_build(std::vector<pArray> sourceArr)
{
	getSourceArrayIfEmpty(sourceArr);

	eleDefault level = 5;
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	// Execute mmt build action
	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	return afterArray;
}

pArray mmt_delta_encode(std::vector<pArray> sourceArr)
{
	// Should build mmt before
	getSourceArrayIfEmpty(sourceArr);

	std::shared_ptr<mmt_delta_encode_plan> mmtPlan;
	std::shared_ptr<mmt_delta_encode_action> mmtAction;
	pQuery mmtQuery;
	getMmtDeltaEncode(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);
	std::cout << "mmt delta encode" << std::endl;

	return afterArray;
}
}	// data2D_star1024x1024
}	// caDummy
}	// msdb