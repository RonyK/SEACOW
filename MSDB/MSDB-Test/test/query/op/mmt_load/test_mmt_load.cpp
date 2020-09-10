#include <pch.h>
#include <index/testMMT.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_body_mmt_build(_pFuncGetSourceArray_, eleDefault mmtLevel)
{
    bool printFlag = false;

    std::vector<pArray> sourceArr;
    getSourceArrayIfEmpty(sourceArr);
    sourceArr[0]->setId(sourceArr[0]->getId() + 2);

    auto outArr = mmt_build(sourceArr, mmtLevel);
    std::cout << "##############################" << std::endl;

    outArr = mmt_save(std::vector<pArray>({ outArr }));
    std::cout << "##############################" << std::endl;

    std::cout << "====================" << std::endl;
    std::cout << "Setup for load" << std::endl;
    attributeId attrId = 0;
    arrayMgr::instance()->flushAttributeIndex(sourceArr[0]->getId(), attrId);
    std::cout << "flush attribute index success" << std::endl;
    std::cout << "====================" << std::endl;

    outArr = mmt_load(std::vector<pArray>({ outArr }));
    std::cout << "##############################" << std::endl;

    //EXPECT_TRUE(false);
    return outArr;
}

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
    //auto arr = mmt_build(std::vector<pArray>(), mmtLevel);
    //arr = mmt_save(std::vector<pArray>({ arr }));

    //std::cout << "====================" << std::endl;
    //std::cout << "Setup for load" << std::endl;
    //attributeId attrId = 0;
    //arrayMgr::instance()->flushAttributeIndex(aid, attrId);
    //std::cout << "flush attribute index success" << std::endl;
    //std::cout << "====================" << std::endl;

    //arr = mmt_load(std::vector<pArray>({ arr }));
    auto outArr = test_body_mmt_build<value_type>(&getSourceArrayIfEmpty, mmtLevel);

}	// TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb