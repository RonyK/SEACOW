#include <pch.h>
#include <compression/test_action_compression.h>

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

namespace msdb
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

pArray exe_act_ind_spiht_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<spiht_decode_plan> spihtPlan;
	std::shared_ptr<spiht_decode_action> spihtAction;
	pQuery spihtQuery;
	getSPIHTDecode(sourceArr[0]->getDesc(), spihtPlan, spihtAction, spihtQuery);

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
}	// caDummy
}	// msdbk
