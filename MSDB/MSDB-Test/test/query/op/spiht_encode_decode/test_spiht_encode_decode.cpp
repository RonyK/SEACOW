#include <pch.h>
#include <compression/test_action_compression.h>
#include <compression/test_qry_spiht.h>
#include <util/timer.h>

namespace msdb
{
namespace caDummy
{
template <typename value_type>
pArray test_qry_ind_spiht_encode(_pFuncGetSourceArray_, eleDefault wtLevel, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + 4);

	auto outArr = qry_exe_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_spiht_encode_decode(_pFuncGetSourceArray_, eleDefault wtLevel)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + 4);

	qry_exe_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	auto outArr = qry_exe_ind_spiht_decode<value_type>(sourceArr, wtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	BOOST_LOG_TRIVIAL(debug) << "Array: " << sourceArr[0]->getDesc()->name_;
	return outArr;
}

template <typename value_type>
pArray test_qry_seq_spiht_encode_decode(_pFuncGetSourceArray_, eleDefault wtLevel)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + 4);

	qry_exe_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	auto outArr = qry_exe_seq_spiht_decode<value_type>(sourceArr, wtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	BOOST_LOG_TRIVIAL(debug) << "Array: " << sourceArr[0]->getDesc()->name_;
	return outArr;
}
namespace data2D_sc4x4
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_ind_sc4x4)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_seq_sc4x4)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_sc4x4

namespace data2D_star1024x1024
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_ind_star1024x1024)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_seq_star1024x1024)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_ind_saturn1024x1024)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_seq_saturn1024x1024)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_spiht_encode_decode, spiht_encode_decode_ind_solar1024x1024)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_seq_solar1024x1024)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_spiht_encode, spiht_encode_ind_mars4096x2048)
{
	bool printFlag = true;
	test_qry_ind_spiht_encode<value_type>(&getSourceArrayIfEmpty, wtLevel, printFlag);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_ind_mars4096x2048)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}

TEST(query_op_spiht_encode_decode, spiht_encode_decode_seq_mars4096x2048)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, wtLevel);
}
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb