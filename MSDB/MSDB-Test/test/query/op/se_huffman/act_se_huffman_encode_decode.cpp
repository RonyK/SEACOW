#include <pch.h>
#include <dummy/opDummy.h>
#include "act_se_huffman_encode_decode.h"

namespace msdb
{
namespace caDummy
{
void get_op_se_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<se_huffman_encode_plan>& plan, std::shared_ptr<se_huffman_encode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<se_huffman_encode_plan, se_huffman_encode_action, se_huffman_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void get_op_se_huffman_decode(pArrayDesc sourceArrDesc, eleDefault level, std::shared_ptr<se_huffman_decode_plan>& plan, std::shared_ptr<se_huffman_decode_action>& action, pQuery& qry)
{
	auto planAction = getLevelArrayParamOperator<se_huffman_decode_plan, se_huffman_decode_action, se_huffman_decode_array_pset>(
		sourceArrDesc, level, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

pArray exe_act_ind_se_huffman_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<se_huffman_encode_plan> myPlan;
	std::shared_ptr<se_huffman_encode_action> myAction;
	pQuery myQuery;
	get_op_se_huffman_encode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_se_huffman_decode(std::vector<pArray> sourceArr, eleDefault level)
{
	std::shared_ptr<se_huffman_decode_plan> myPlan;
	std::shared_ptr<se_huffman_decode_action> myAction;
	pQuery myQuery;
	get_op_se_huffman_decode(sourceArr[0]->getDesc(), level, myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}

std::shared_ptr<se_huffman_encode_plan> getSeHuffmanEncodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<se_huffman_encode_plan, se_huffman_encode_array_pset>(sourceArrDesc, wtLevel, qry);
}

std::shared_ptr<se_huffman_decode_plan> getSeHuffmanDecodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry)
{
	return getLevelArrayParamPlan<se_huffman_decode_plan, se_huffman_decode_array_pset>(sourceArrDesc, wtLevel, qry);
}
}   // caDummy
}   // msdb

