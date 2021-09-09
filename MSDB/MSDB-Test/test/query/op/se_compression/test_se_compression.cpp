#include <pch_test.h>

#include <compression/test_action_compression.h>
#include <compression/test_qry_secompression.h>
#include <index/test_qry_mmt.h>
#include <system/storageMgr.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
namespace se_comp_decomp
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;
}

template <typename value_type>
pArray test_body_ind_spiht_delte_encode(_pFuncGetSourceArray_,
									   eleDefault wtLevel,
									   eleDefault mmtLevel,
									   bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + delta_spiht_array_id);

	auto outArr = exe_qry_ind_delta_spiht_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_body_ind_spiht_delte_encode_decode(_pFuncGetSourceArray_,
											  eleDefault wtLevel,
											  eleDefault mmtLevel,
											  bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + delta_spiht_array_id);

	exe_qry_ind_delta_spiht_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = exe_qry_ind_delta_spiht_decode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	BOOST_LOG_TRIVIAL(debug) << "Array: " << sourceArr[0]->getDesc()->name_;

	return outArr;
}

namespace data2D_sc4x4
{
TEST(query_op_se_compression, sc4x4_ind_se_comp_decomp)
{
	test_body_ind_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel);		// 443
}

TEST(query_op_se_compression, sc4x4_ind_se_comp)
{
	test_body_ind_se_comp<value_type>(&getSourceArrayIfEmpty,
									&getSourceArrayDesc,
									wtLevel, mmtLevel);		// 443
}

TEST(query_op_se_compression, sc4x4_seq_se_comp_decomp)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::printFlag);				// 443
}
}   // data2D_sc4x4

namespace data2D_test32x32
{
TEST(query_op_se_compression, test32x32_seq_se_compr_decomp)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::printFlag);						// 19362
}	// TEST()
}	// data2D_test32x32

namespace data2D_tempTest
{
TEST(query_op_se_compression, tempTest_ind_se_compr_decomp)
{
	test_body_ind_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::printFlag);		// 19422
}	// TEST()
}	// data2D_tempTest

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, star1024x1024_ind_se_compr_decomp)
{
	test_body_ind_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel, 
											 se_comp_decomp::printFlag);		// 19422
}	// TEST()

TEST(experiment_query_op_se_compr_decomp, star1024x1024_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);				// 19422
}	// TEST()

TEST(query_op_spiht_delta, star1024x1024_ind_spiht_delta_encode_decode)
{
	test_body_ind_spiht_delte_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   wtLevel, mmtLevel,
														se_comp_decomp::printFlag);
}	// TEST()
}   // data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(experiment_query_op_se_compr_decomp, saturn1014x1024_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);						// 19362
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(experiment_query_op_se_compr_decomp, solar1024x1024_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);						// 19362
}	// TEST()
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(experiment_query_op_se_compr_decomp, mars4096x2048_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);						// 19362
}	// TEST()
TEST(query_op_spiht_delta, mars4096x2048_ind_spiht_delta_encode_decode)
{
	test_body_ind_spiht_delte_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   wtLevel, mmtLevel,
														se_comp_decomp::printFlag);				// 19362
}	// TEST()

TEST(query_op_spiht_delta, mars4096x2048_ind_spiht_delta_encode)
{
	test_body_ind_spiht_delte_encode<value_type>(&getSourceArrayIfEmpty,
												wtLevel, mmtLevel,
												 se_comp_decomp::printFlag);						// 19362
}	// TEST()
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(experiment_query_op_se_compr_decomp, mercurydem20480x10240_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);						// 20102
}	// TEST()
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(experiment_query_op_se_compr_decomp, lunar102400x40960_seq)
{
	test_body_seq_se_comp_decomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 se_comp_decomp::validation, se_comp_decomp::printFlag);						// 20102
}		// TEST()
}		// data2D_lunar102400x40960
}		// caDummy
}		// core
}		// msdb