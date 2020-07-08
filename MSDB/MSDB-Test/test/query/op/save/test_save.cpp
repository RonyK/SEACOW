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
TEST(query_op_save, save_sc4x4)
{
    auto arr = save(std::vector<pArray>());
}	// TEST()
}	// data2D_sc4x4
}	// caDummy
}	// msdb