#include <pch.h>
#include <array/arrayMgr.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>
#include <index/mmt.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(mmt_build_action, mmt_build_sc4x4)
{
	std::vector<pArray> sourceArr = getSourceArray();
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
}	// TEST()
}	// data2D_sc4x4

namespace data2D_sc8x8
{
TEST(mmt_build_action, mmt_build_sc8x8)
{
	std::vector<pArray> sourceArr = getSourceArray();
	eleDefault level = 2;
	std::shared_ptr<mmt_build_plan> mmtPlan;
	std::shared_ptr<mmt_build_action> mmtAction;
	pQuery mmtQuery;
	getMmtBuild(sourceArr[0]->getDesc(), level, mmtPlan, mmtAction, mmtQuery);

	// Execute mmt build action
	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

	// Result check
	for(auto attrDesc : afterArray->getDesc()->attrDescs_)
	{
		auto attrIndex = arrayMgr::instance()->getAttributeIndex(afterArray->getArrayId(), attrDesc->id_);
		EXPECT_TRUE(attrIndex != nullptr);

		std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(attrIndex);
		auto nodes = mmtIndex->getNodes();
		EXPECT_EQ(nodes.size(), level + 1);

		value_type exMMT_min[3][4][4];
		value_type exMMT_max[3][4][4];
		getExMMTBuilded(exMMT_min, exMMT_max, 3 * 4 * 4);

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
}	// TEST()
}	// data2D_sc8x8
}	// caDummy
}	// msdb