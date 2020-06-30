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

#include <string>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if(sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}

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

	// Result check
	for (auto attrDesc : afterArray->getDesc()->attrDescs_)
	{
		auto attrIndex = arrayMgr::instance()->getAttributeIndex(afterArray->getArrayId(), attrDesc->id_);
		EXPECT_TRUE(attrIndex != nullptr);

		std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(attrIndex);
		auto nodes = mmtIndex->getNodes();
		EXPECT_EQ(nodes.size(), level + 1);

		value_type exMMT_min[2][2][2];
		value_type exMMT_max[2][2][2];
		getExMMTBuilded(exMMT_min, exMMT_max, 2 * 2 * 2);

		for (size_t l = 0; l <= level; l++)
		{
			auto levelNodes = nodes[l];
			MinMaxTreeImpl<dim_type, value_type>::nodeItr nit(levelNodes.data(), 2, mmtIndex->getLevelDim(l).data());
			for (int y = 0; y < chunkNums[0] / pow(2, l); ++y)
			{
				for (int x = 0; x < chunkNums[1] / pow(2, l); ++x)
				{
					coor blockCoor({ y, x });
					nit.moveTo(blockCoor);
					auto node = *nit;

					std::cout << "[" << x << ", " << y << "] " << static_cast<int>(node->min_) << " ~ " << static_cast<int>(node->max_) << std::endl;

					EXPECT_EQ(exMMT_min[l][y][x], node->min_);
					EXPECT_EQ(exMMT_max[l][y][x], node->max_);
				}
			}
		}
	}

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
}	// data2D_sc4x4
}	// caDummy
}	// msdb