#include <pch.h>
#include <compression/test_action_compression.h>
#include <compression/test_qry_spiht.h>
#include <util/timer.h>

namespace msdb
{
namespace caDummy
{
const int spiht_array_id = 4;

template <typename value_type>
pArray test_qry_ind_spiht_encode(_pFuncGetSourceArray_, 
								 _pFuncGetSourceArrayDesc_, 
								 eleDefault wtLevel, bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + spiht_array_id);

	auto outArr = exe_qry_ind_spiht_encode<value_type>(sourceArrDesc, wtLevel, printFlag);

	return outArr;
}

template <typename value_type>
pArray test_qry_ind_spiht_encode_decode(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_, 
										eleDefault wtLevel)
{
	bool printFlag = false;
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
										bool printFlag = false)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);
	sourceArr[0]->setId(sourceArr[0]->getId() + spiht_array_id);

	auto sourceArrDesc = getArrayFromFunction<value_type>(getSourceArrayDesc, printFlag);
	sourceArrDesc[0]->setId(sourceArrDesc[0]->getId() + spiht_array_id);

	exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, printFlag);
	 pArray outArr = exe_qry_seq_spiht_decode<value_type>(sourceArrDesc, wtLevel, printFlag);

	compArrary<value_type>(sourceArr[0], outArr);
	BOOST_LOG_TRIVIAL(debug) << "Array: " << sourceArr[0]->getDesc()->name_;

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
												 wtLevel);
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

TEST(query_op_spiht_encode_decode, star1024x1024_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, 
												 wtLevel);
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

TEST(query_op_spiht_encode_decode, saturn1024x1024_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, 
												 wtLevel);
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

TEST(query_op_spiht_encode_decode, solar1024x1024_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc, 
												 wtLevel);
}
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_spiht_encode, mars4096x2048_ind_spiht_encode)
{
	bool printFlag = false;
	test_qry_ind_spiht_encode<value_type>(&getSourceArrayIfEmpty, 
										  &getSourceArrayDesc, 
										  wtLevel, printFlag);
}

TEST(query_op_spiht_encode_decode, mars4096x2048_ind_spiht_encode_decode)
{
	test_qry_ind_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, 
												 &getSourceArrayDesc, 
												 wtLevel);
}

TEST(query_op_spiht_encode_decode, mars4096x2048_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, 
												 &getSourceArrayDesc, 
												 wtLevel);
}
}	// data2D_mars4096x2048

namespace data2D_mercurydem20480x10240
{
TEST(query_op_spiht_encode_decode, mercurydem20480x10240_seq_spiht_encode_decode)
{
	test_qry_seq_spiht_encode_decode<value_type>(&getSourceArrayIfEmpty, 
												 &getSourceArrayDesc, 
												 wtLevel);
}
}	// data2D_mercurydem20480x10240
}	// caDummy
}	// msdb