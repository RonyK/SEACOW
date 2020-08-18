#include <pch.h>
#include <index/testMMT.h>
#include <system/storageMgr.h>
#include <io/testIO.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_sc4x4
{
TEST(query_op_load, load_sc4x4)
{
    auto saveArr = save(std::vector<pArray>(), &getSourceArrayIfEmpty);
    
    auto loadArrDesc = std::make_shared<arrayDesc>(*saveArr->getDesc());
    saveArr = nullptr;

    auto loadArrSchema = std::make_shared<arrayBase>(loadArrDesc);
    auto loadArr = load(std::vector<pArray>({ loadArrSchema }), &getSourceArrayIfEmpty);
    load_test(loadArr);
}	// TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb