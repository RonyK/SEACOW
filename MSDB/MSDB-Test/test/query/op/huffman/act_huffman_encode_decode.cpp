#include <pch_test.h>
#include <dummy/opDummy.h>
#include "act_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
void get_op_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<huffman_encode_plan>& plan, std::shared_ptr<huffman_encode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<huffman_encode_plan, huffman_encode_action, huffman_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void get_op_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<huffman_decode_plan>& plan, std::shared_ptr<huffman_decode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<huffman_decode_plan, huffman_decode_action, huffman_decode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

pArray exe_act_ind_huffman_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<huffman_encode_plan> myPlan;
	std::shared_ptr<huffman_encode_action> myAction;
	pQuery myQuery;
	get_op_huffman_encode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_huffman_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<huffman_decode_plan> myPlan;
	std::shared_ptr<huffman_decode_action> myAction;
	pQuery myQuery;
	get_op_huffman_decode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}

std::shared_ptr<huffman_encode_plan> getHuffmanEncodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<huffman_encode_plan, huffman_encode_array_pset>(sourceArrDesc, qry);
}

std::shared_ptr<huffman_decode_plan> getHuffmanDecodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<huffman_decode_plan, huffman_decode_array_pset>(sourceArrDesc, qry);
}
}		// caDummy
}		// core
}		// msdb