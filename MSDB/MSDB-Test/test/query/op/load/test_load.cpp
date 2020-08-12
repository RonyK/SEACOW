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
    auto loadArr = load(std::vector<pArray>({saveArr}), &getSourceArrayIfEmpty);
    load_test(loadArr);
}	// TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb