#include <pch.h>

#include <compression/test_action_compression.h>
#include <compression/test_qry_secompression.h>
#include <index/test_qry_mmt.h>
#include <system/storageMgr.h>

namespace msdb
{
namespace caDummy
{
const int se_array_id = 2;
const int delta_spiht_array_id = 3;
template <typename value_type>
pArray test_qry_ind_secomp_sedecomp(_pFuncGetSourceArray_,
									_pFuncGetSourceArrayDesc_,
									eleDefault wtLevel, eleDefault mmtLevel)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
	exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = exe_qry_ind_se_decompression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	return outArr;
}

template <typename value_type>
pArray test_qry_seq_secomp_sedecomp(_pFuncGetSourceArray_,
									_pFuncGetSourceArrayDesc_,
									eleDefault wtLevel,
									eleDefault mmtLevel,
									bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + se_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + se_array_id);

	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, printFlag);
	exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = exe_qry_seq_se_decompression<value_type>(sourceArrDesc, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	return outArr;
}

template <typename value_type>
pArray test_qry_ind_delte_spiht_encode(_pFuncGetSourceArray_,
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
pArray test_qry_ind_delte_spiht_encode_decode(_pFuncGetSourceArray_,
											  eleDefault wtLevel,
											  eleDefault mmtLevel,
											  bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + delta_spiht_array_id);

	exe_qry_ind_delta_spiht_encode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);
	auto outArr = exe_qry_ind_delta_spiht_decode<value_type>(sourceArr, wtLevel, mmtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);

	return outArr;
}

namespace data2D_sc4x4
{
TEST(query_op_se_compression, se_compression_sc4x4)
{
	test_qry_ind_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel);		// 443
}

TEST(query_op_se_compression, se_comrpession_seq_sc4x4)
{
	bool printFlag = false;
	test_qry_seq_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 printFlag);				// 443
}
}   // data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_se_compression, se_compression_star1024x1024)
{
	test_qry_ind_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel);		// 19422
}	// TEST()

TEST(query_op_se_compression, se_comrpession_seq_star1014x1024)
{
	bool printFlag = false;
	test_qry_seq_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 printFlag);				// 19422
}	// TEST()

TEST(query_op_delta_spiht_compression, delta_spiht_star1024x1024)
{
	bool printFlag = false;
	test_qry_ind_delte_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   wtLevel, mmtLevel,
													   printFlag);
}	// TEST()
}   // data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_se_compression, se_comrpession_seq_saturn1014x1024)
{
	bool printFlag = false;
	test_qry_seq_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 printFlag);						// 19362
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_se_compression, se_comrpession_seq_mars4096x2048)
{
	bool printFlag = false;
	test_qry_seq_secomp_sedecomp<value_type>(&getSourceArrayIfEmpty,
											 &getSourceArrayDesc,
											 wtLevel, mmtLevel,
											 printFlag);						// 19362
}	// TEST()
TEST(query_op_spiht_delta, delta_spiht_encode_decode_mars4096x2048)
{
	bool printFlag = false;
	test_qry_ind_delte_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
													   wtLevel, mmtLevel,
													   printFlag);						// 19362
}	// TEST()

TEST(query_op_spiht_delta, delta_spiht_encode_mars4096x2048)
{
	bool printFlag = true;
	test_qry_ind_delte_spiht_encode<value_type>(&getSourceArrayIfEmpty,
												wtLevel, mmtLevel,
												printFlag);						// 19362
}	// TEST()
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb