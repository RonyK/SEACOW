#include <pch_test.h>
#include <compression/test_action_compression.h>
#include <compression/getOpCompression.h>

#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/spiht_decode/spiht_decode_action.h>

#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_compression_action.h>
#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

#include <op/compass_encode/compass_encode_plan.h>
#include <op/compass_encode/compass_encode_action.h>
#include <op/compass_decode/compass_decode_plan.h>
#include <op/compass_decode/compass_decode_action.h>
#include <op/compass_index_build/compass_index_build_plan.h>
#include <op/compass_index_build/compass_index_build_action.h>

#include <op/lzw_encode/lzw_encode_plan.h>
#include <op/lzw_encode/lzw_encode_action.h>
#include <op/lzw_decode/lzw_decode_plan.h>
#include <op/lzw_decode/lzw_decode_action.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
pArray exe_act_ind_wavelet_encode(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<wavelet_encode_plan> wePlan;
	std::shared_ptr<wavelet_encode_action> weAction;
	pQuery weQuery;
	getWaveletEncode(sourceArr[0]->getDesc(), level, wePlan, weAction, weQuery);

	return weAction->execute(sourceArr, weQuery);
}

pArray exe_act_ind_wavelet_decode(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<wavelet_decode_plan> wdPlan;
	std::shared_ptr<wavelet_decode_action> wdAction;
	pQuery wdQuery;
	getWaveletDecode(sourceArr[0]->getDesc(), level, wdPlan, wdAction, wdQuery);

	return wdAction->execute(sourceArr, wdQuery);
}

pArray exe_act_ind_spiht_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<spiht_encode_plan> spihtPlan;
	std::shared_ptr<spiht_encode_action> spihtAction;
	pQuery spihtQuery;
	getSPIHTEncode(sourceArr[0]->getDesc(), spihtPlan, spihtAction, spihtQuery);

	return spihtAction->execute(sourceArr, spihtQuery);
}

pArray exe_act_ind_spiht_decode(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<spiht_decode_plan> spihtPlan;
	std::shared_ptr<spiht_decode_action> spihtAction;
	pQuery spihtQuery;
	getSPIHTDecode(sourceArr[0]->getDesc(), level, spihtPlan, spihtAction, spihtQuery);

	return spihtAction->execute(sourceArr, spihtQuery);
}

pArray exe_act_ind_se_compression(std::vector<pArray> sourceArr)
{
	std::shared_ptr<se_compression_plan> sePlan;
	std::shared_ptr<se_compression_action> seAction;
	pQuery seQuery;
	getSeCompression(sourceArr[0]->getDesc(), sePlan, seAction, seQuery);

	auto afterArray = seAction->execute(sourceArr, seQuery);

	return afterArray;
}

pArray exe_act_ind_se_decompression(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<se_decompression_plan> sePlan;
	std::shared_ptr<se_decompression_action> seAction;
	pQuery seQuery;
	getSeDecompression(sourceArr[0]->getDesc(), level, sePlan, seAction, seQuery);

	auto afterArray = seAction->execute(sourceArr, seQuery);

	return afterArray;
}
pArray exe_act_ind_compass_encode(std::vector<pArray> sourceArr, eleDefault numBins)
{
	std::shared_ptr<compass_encode_plan> myPlan;
	std::shared_ptr<compass_encode_action> myAction;
	pQuery myQuery;
	getCompassEncode(sourceArr[0]->getDesc(), numBins, myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_compass_decode(std::vector<pArray> sourceArr, eleDefault numBins)
{
	std::shared_ptr<compass_decode_plan> myPlan;
	std::shared_ptr<compass_decode_action> myAction;
	pQuery myQuery;
	getCompassDecode(sourceArr[0]->getDesc(), numBins, myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_compass_index_build(std::vector<pArray> sourceArr, eleDefault numBins)
{
	std::shared_ptr<compass_index_build_plan> myPlan;
	std::shared_ptr<compass_index_build_action> myAction;
	pQuery myQuery;
	getCompassIndexBuild(sourceArr[0]->getDesc(), numBins, myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_zip_save(std::vector<pArray> sourceArr)
{
	std::shared_ptr<zip_save_plan> myPlan;
	std::shared_ptr<zip_save_action> myAction;
	pQuery myQuery;
	getOpZipSave(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_zip_load(std::vector<pArray> sourceArr)
{
	std::shared_ptr<zip_load_plan> myPlan;
	std::shared_ptr<zip_load_action> myAction;
	pQuery myQuery;
	getOpZipLoad(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_lzw_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<lzw_encode_plan> myPlan;
	std::shared_ptr<lzw_encode_action> myAction;
	pQuery myQuery;
	getOpLzwEncode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_lzw_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<lzw_decode_plan> myPlan;
	std::shared_ptr<lzw_decode_action> myAction;
	pQuery myQuery;
	getOpLzwDecode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
}		// caDummy
}		// core
}		// msdb
