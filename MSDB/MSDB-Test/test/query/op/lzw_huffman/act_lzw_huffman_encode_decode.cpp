#include <pch.h>
#include "act_lzw_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
void get_op_lzw_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<lzw_huffman_encode_plan>& plan, std::shared_ptr<lzw_huffman_encode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<lzw_huffman_encode_plan, lzw_huffman_encode_action, lzw_huffman_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void get_op_lzw_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<lzw_huffman_decode_plan>& plan, std::shared_ptr<lzw_huffman_decode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<lzw_huffman_decode_plan, lzw_huffman_decode_action, lzw_huffman_decode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

pArray exe_act_ind_lzw_huffman_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<lzw_huffman_encode_plan> myPlan;
	std::shared_ptr<lzw_huffman_encode_action> myAction;
	pQuery myQuery;
	get_op_lzw_huffman_encode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_lzw_huffman_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<lzw_huffman_decode_plan> myPlan;
	std::shared_ptr<lzw_huffman_decode_action> myAction;
	pQuery myQuery;
	get_op_lzw_huffman_decode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}

std::shared_ptr<lzw_huffman_encode_plan> getLzwHuffmanEncodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<lzw_huffman_encode_plan, lzw_huffman_encode_array_pset>(sourceArrDesc, qry);
}

std::shared_ptr<lzw_huffman_decode_plan> getLzwHuffmanDecodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<lzw_huffman_decode_plan, lzw_huffman_decode_array_pset>(sourceArrDesc, qry);
}
}		// caDummy
}		// core
}		// msdb