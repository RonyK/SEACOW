#include <pch.h>
#include <array/arrayMgr.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_build/mmt_build_action.h>
#include <system/storageMgr.h>
#include <index/mmt.h>
#include <string>

namespace msdb
{
namespace caDummy
{

class query_op_mmt_save : public ::testing::Test
{
protected:
    // You can define per-test set-up logic as usual.
    virtual void SetUp()
    {
        std::cout << "====================" << std::endl;
        std::cout << "Setup()" << std::endl;
        try
        {
            std::filesystem::remove_all(
                filePath(strArrayPath) / std::to_string(data2D_sc4x4::aid) / strIndexFolder);
            std::filesystem::remove_all(
                filePath(strArrayPath) / std::to_string(data2D_sc8x8::aid) / strIndexFolder);

            std::cout << "Remove index file success" << std::endl;
        } catch (...)
        {
            std::cout << "Remove sc4x4 array indies fail" << std::endl;
        }
        std::cout << "====================" << std::endl;
    }

    // You can define per-test tear-down logic as usual.
    virtual void TearDown()
    {
        std::cout << "====================" << std::endl;
        std::cout << "TearDown()" << std::endl;
        std::cout << "====================" << std::endl;
    }
};  // class query_op_mmt_save

namespace data2D_sc4x4
{
TEST_F(query_op_mmt_save, mmt_serialize_sc4x4)
{
    // Should build mmt before
    std::vector<pArray> sourceArr = getSourceArray();
    pArray arr = sourceArr[0];
    arrayId arrId = arr->getArrayId();

    for (auto attr : arr->getDesc()->attrDescs_)
    {
        auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrId, attr->id_);
        if (arrIndex->getType() != attrIndexType::MMT)
        {
            _MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
        }
        std::shared_ptr<MinMaxTreeImpl<position_t, char>> mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, char>>(arrIndex);
        bstream bs;
        mmtIndex->serialize(bs);
    }
}

TEST_F(query_op_mmt_save, mmt_save_sc4x4)
{
	// Should build mmt before
	std::vector<pArray> sourceArr = getSourceArray();
	std::shared_ptr<mmt_save_plan> mmtPlan;
	std::shared_ptr<mmt_save_action> mmtAction;
	pQuery mmtQuery;
	getMmtSave(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

	auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

    EXPECT_TRUE(std::filesystem::is_regular_file(
        filePath("../storage/array/441/indies/0.msdbindex")));
}	// TEST()
}	// data2D_sc4x4

namespace data2D_sc8x8
{
TEST_F(query_op_mmt_save, mmt_save_sc8x8)
{
    // Should build mmt before
    std::vector<pArray> sourceArr = getSourceArray();
    std::shared_ptr<mmt_save_plan> mmtPlan;
    std::shared_ptr<mmt_save_action> mmtAction;
    pQuery mmtQuery;
    getMmtSave(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

    auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

    EXPECT_TRUE(std::filesystem::is_regular_file(
        filePath("../storage/array/881/indies/0.msdbindex")));
}	// TEST()
}	// data2D_sc8x8
}	// caDummy
}	// msdb