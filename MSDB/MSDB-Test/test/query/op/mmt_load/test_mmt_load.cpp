#include <pch.h>
#include <index/test_action_mmt.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace mmt_save_load
{
bool validation = true;
bool printFlag = false;
}

template <typename dim_type, typename value_type>
bool compMmt(std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>>saveMmtIndex,
			 std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>>loadMmtIndex)
{
	return saveMmtIndex == loadMmtIndex;
}

template <typename value_type>
pArray exe_qry_ind_mmt_save(_vectorSourceArray_, pAttrIndex& outIndex, eleDefault mmtLevel, bool printFlag)
{
	attributeId attrId = 0;

	//////////////////////////////
	// 01. Get Source Array
	auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	//////////////////////////////

	//////////////////////////////
	// 02. Save MMT
	auto outArr = exe_act_ind_mmt_save(std::vector<pArray>({ arr_mmt_build }));
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	//////////////////////////////

	//////////////////////////////
	// 03. Get MMT Test
	outIndex = arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), attrId);
	assert(outIndex != nullptr);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_mmt_load(_vectorSourceArray_, pAttrIndex& outIndex, bool printFlag)
{
	attributeId attrId = 0;

	//////////////////////////////
	// 01. Flush Attribute Index
	BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
	BOOST_LOG_TRIVIAL(debug) << "Setup for load" << std::endl;
	arrayMgr::instance()->flushAttributeIndex(sourceArr[0]->getId(), attrId);
	BOOST_LOG_TRIVIAL(debug) << "flush attribute index success" << std::endl;
	BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;

	//////////////////////////////
	// 02. Load MMT
	pQuery qry = std::make_shared<query>();

	auto mmtLoadPlan = getMMTLoadPlan(sourceArr[0]->getDesc(), qry);

	auto outArr = mmtLoadPlan->getAction()->execute(sourceArr, qry);
	if (printFlag)
	{
		BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "MMT Load Arr" << std::endl;
		outArr->print();
	}
	//////////////////////////////

	//////////////////////////////
	// 03. Get MMT Test
	outIndex = arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), attrId);
	assert(outIndex != nullptr);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_mmt_save_load(_pFuncGetSourceArray_, eleDefault mmtLevel,
								   bool validation, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);
	//////////////////////////////

	//////////////////////////////
	// 02. Save MMT
	pAttrIndex outSaveIndex = nullptr;
	exe_qry_ind_mmt_save<value_type>(sourceArr, outSaveIndex, mmtLevel, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 03. Load MMT
	pAttrIndex outLoadIndex = nullptr;
	auto outArr = exe_qry_seq_mmt_load<value_type>(sourceArr, outLoadIndex, printFlag);
	//////////////////////////////

	//////////////////////////////
	// 04. Validation
	if (validation)
	{
		std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> saveMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(outSaveIndex);
		std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> loadMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(outLoadIndex);

		compMmt<dim_type, value_type>(saveMmtIndex, loadMmtIndex);
	}
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_mmt_save_load(_pFuncGetSourceArray_, eleDefault mmtLevel)
{
	attributeId attrId = 0;
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

	auto arr_mmt_build = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	auto arr_mmt_save = exe_act_ind_mmt_save(std::vector<pArray>({ arr_mmt_build }));
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	auto saveIndex = arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), attrId);

	//////////////////////////////
	BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
	BOOST_LOG_TRIVIAL(debug) << "Setup for load" << std::endl;
	arrayMgr::instance()->flushAttributeIndex(sourceArr[0]->getId(), attrId);
	BOOST_LOG_TRIVIAL(debug) << "flush attribute index success" << std::endl;
	BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;

	//////////////////////////////
	auto arr_mmt_load = exe_act_ind_mmt_load(sourceArr);
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;
	auto loadIndex = arrayMgr::instance()->getAttributeIndex(sourceArr[0]->getId(), attrId);

	//////////////////////////////
	std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> saveMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(saveIndex);
	std::shared_ptr<MinMaxTreeImpl<dim_type, value_type>> loadMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<dim_type, value_type>>(loadIndex);

	compMmt<dim_type, value_type>(saveMmtIndex, loadMmtIndex);
	//EXPECT_TRUE(false);

	return arr_mmt_load;
}

template <typename value_type>
pArray test_body_mmt_build(_pFuncGetSourceArray_, eleDefault mmtLevel)
{
	bool printFlag = false;

	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);

	auto outArr = exe_act_ind_mmt_build(sourceArr, mmtLevel);
	BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;

	//outArr = exe_act_ind_mmt_save(std::vector<pArray>({ outArr }));
	//BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;

	//BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
	//BOOST_LOG_TRIVIAL(debug) << "Setup for load" << std::endl;
	//attributeId attrId = 0;
	//arrayMgr::instance()->flushAttributeIndex(sourceArr[0]->getId(), attrId);
	//BOOST_LOG_TRIVIAL(debug) << "flush attribute index success" << std::endl;
	//BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;

	//outArr = exe_act_ind_mmt_load(std::vector<pArray>({ outArr }));
	//BOOST_LOG_TRIVIAL(debug) << "##############################" << std::endl;

	//EXPECT_TRUE(false);
	return outArr;
}

class query_op_mmt_load : public ::testing::Test
{
protected:
	// You can define per-test set-up logic as usual.
	virtual void SetUp()
	{
		BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "Setup()" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
	}

	// You can define per-test tear-down logic as usual.
	virtual void TearDown()
	{
		BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "TearDown()" << std::endl;
		BOOST_LOG_TRIVIAL(debug) << "====================" << std::endl;
	}
};  // class query_op_mmt_load

namespace data2D_sc4x4
{
TEST_F(query_op_mmt_load, mmt_save_load_sc4x4)
{
	auto outArr = test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);
}	// TEST()
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST_F(query_op_mmt_load, star1024x1024_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, star1024x1024_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation, mmt_save_load::printFlag);
}   // TEST
}   // data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST_F(query_op_mmt_load, saturn1024x1024_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, saturn1024x1024_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation, mmt_save_load::printFlag);
}   // TEST
}   // data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST_F(query_op_mmt_load, solar1024x1024_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, solar1024x1024_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation, mmt_save_load::printFlag);
}   // TEST
}   // solar1024x1024

namespace data2D_mars4096x2048
{
TEST_F(query_op_mmt_load, mars4096x2048_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, mars4096x2048_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation, mmt_save_load::printFlag);
}   // TEST
}   // data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST_F(query_op_mmt_load, mercurydem20480x10240_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, mercurydem20480x10240_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation, mmt_save_load::printFlag);
}   // TEST
}   // data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST_F(query_op_mmt_load, lunar102400x40960_mmt_save_load)
{
	test_body_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel);        // 24243
}   // TEST

TEST(experiment_query_op_mmt_save_load, lunar102400x40960_seq)
{
	test_body_seq_mmt_save_load<value_type>(&getSourceArrayIfEmpty, mmtLevel,
											mmt_save_load::validation,
											mmt_save_load::printFlag);
}	// TEST
}	// data2D_lunar102400x40960
}		// caDummy
}		// core
}		// msdb