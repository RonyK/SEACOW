#include <pch.h>
#include <compression/test_action_compression.h>
#include <compression/test_qry_spiht.h>
#include <util/timer.h>

namespace msdb
{
namespace caDummy
{
namespace spiht_encode_decode
{
	bool saveArray = false;
	bool printFlag = false;
	bool validation = true;
}

template <typename value_type>
pArray test_qry_ind_spiht_encode(_pFuncGetSourceArray_,
								 _pFuncGetSourceArrayDesc_,
								 eleDefault wtLevel, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + spiht_array_id);

	auto outArr = exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_spiht_encode_decode(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault wtLevel, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + spiht_array_id);

	exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	auto outArr = exe_qry_ind_spiht_decode<value_type>(sourceArrDesc, wtLevel, printFlag);

	//compArrary<value_type>(sourceArr[0], outArr);

	return outArr;
}

template <typename value_type>
pArray test_qry_seq_spiht_encode_decode(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault wtLevel,
										bool validation = false, bool printFlag = false)
{
	//////////////////////
	// 01. Get Array
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, false);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, false);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + spiht_array_id);
	//////////////////////

	//////////////////////
	// 02. Encode
	exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	//////////////////////

	//////////////////////
	// 03. Decode
	pArray outArr = exe_qry_seq_spiht_decode<value_type>(sourceArrDesc, wtLevel, printFlag);
	//////////////////////

	//////////////////////
	// 04. Validation
	if(validation)
	{
		compArrary<value_type>(sourceArr[0], outArr);
		BOOST_LOG_TRIVIAL(debug) << "validation finished";
	}
	//////////////////////

	return outArr;
}
namespace data2D_sc4x4
{
TEST(query_op_spiht_encode_decode, sc4x4_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel);
}

TEST(query_op_spiht_encode_decode, sc4x4_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_spiht_encode_decode, star1024x1024_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel);
}

TEST(experiment_query_op_spiht_encode_decode, star1024x1024_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_spiht_encode_decode, saturn1024x1024_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel);
}

TEST(experiment_query_op_spiht_encode_decode, saturn1024x1024_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_spiht_encode_decode, solar1024x1024_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel);
}

TEST(experiment_query_op_spiht_encode_decode, solar1024x1024_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_spiht_encode, mars4096x2048_ind_spiht_encode)
{
	test_qry_ind_spiht_encode<value_type>(&getSourceArrayIfEmpty,
										  &getSourceArrayDesc,
										  wtLevel, spiht_encode_decode::printFlag);
}

TEST(query_op_spiht_encode_decode, mars4096x2048_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel);
}

TEST(experiment_query_op_spiht_encode_decode, mars4096x2048_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(experiment_query_op_spiht_encode_decode, mercurydem20480x10240_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_mercurydem20480x10240

namespace data2D_lunar102400x40960
{
TEST(experiment_query_op_spiht_encode_decode, lunar102400x40960_seq)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 wtLevel, spiht_encode_decode::validation, spiht_encode_decode::printFlag);
}
}	// data2D_lunar102400x40960
}	// caDummy
}	// msdb