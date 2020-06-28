#include <pch.h>
#include <array/arrayMgr.h>
#include <op/mmt_load/mmt_load_plan.h>
#include <op/mmt_load/mmt_load_action.h>
#include <system/storageMgr.h>
#include <index/mmt.h>
#include <string>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
class query_op_mmt_load : public ::testing::Test
{
protected:
    // You can define per-test set-up logic as usual.
    virtual void SetUp()
    {
        std::cout << "====================" << std::endl;
        std::cout << "Setup()" << std::endl;

        attributeId attrId = 0;
        arrayMgr::instance()->flushAttributeIndex(aid, attrId);

        std::cout << "flush attribute index success" << std::endl;

        std::cout << "====================" << std::endl;
    }

    // You can define per-test tear-down logic as usual.
    virtual void TearDown()
    {
        std::cout << "====================" << std::endl;
        std::cout << "TearDown()" << std::endl;
        std::cout << "====================" << std::endl;
    }
};  // class query_op_mmt_load

TEST_F(query_op_mmt_load, mmt_load_sc4x4)
{
    EXPECT_THROW(arrayMgr::instance()->getAttributeIndex(aid, 0), msdb_exception);
    EXPECT_TRUE(std::filesystem::is_regular_file(
        filePath("../storage/array/441/indies/0.msdbindex")));

    // should build mmt before
    std::vector<pArray> sourceArr = getSourceArray();
    std::shared_ptr<mmt_load_plan> mmtPlan;
    std::shared_ptr<mmt_load_action> mmtAction;
    pQuery mmtQuery;
    getMmtLoad(sourceArr[0]->getDesc(), mmtPlan, mmtAction, mmtQuery);

    auto afterArray = mmtAction->execute(sourceArr, mmtQuery);

    auto attrIndex = arrayMgr::instance()->getAttributeIndex(aid, 0);
    std::cout << "mmt load" << std::endl;
}	// TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb