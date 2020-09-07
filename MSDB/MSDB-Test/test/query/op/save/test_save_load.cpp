#include <pch.h>
#include <index/testMMT.h>
#include <system/storageMgr.h>
#include <io/testIO.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
void test_body_save(_pFuncGetSourceArray_)
{
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	auto saveArr = save(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Save Arr" << std::endl;
		saveArr->print();
	}

	auto loadArr = load(std::vector<pArray>({ saveArr }));
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Load Arr" << std::endl;
		loadArr->print();
	}

	compArrary<value_type>(sourceArr[0], loadArr);
	//EXPECT_TRUE(false);
}

namespace data2D_sc4x4
{
TEST(query_op_save_load, save_load_sc4x4)
{
	test_body_save<value_type>(&getSourceArrayIfEmpty);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_save_load, save_load_star1024x1024)
{
	test_body_save<value_type>(&getSourceArrayIfEmpty);
}
}   // data2D_star1024x1024
}	// caDummy
}	// msdb