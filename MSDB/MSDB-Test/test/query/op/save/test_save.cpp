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
    auto arr = save(std::vector<pArray>(), &getSourceArrayIfEmpty);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_save, save_star1024x1024)
{
    auto arr = save(std::vector<pArray>(), &getSourceArrayIfEmpty);
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb