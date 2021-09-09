#include <pch_test.h>
#include <dummy/opDummy.h>
#include "act_adapt_huffman_encode_decode.h"

namespace msdb
{
namespace core
{
namespace caDummy
{
void get_op_adapt_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<adapt_huffman_encode_plan>& plan, std::shared_ptr<adapt_huffman_encode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<adapt_huffman_encode_plan, adapt_huffman_encode_action, adapt_huffman_encode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void get_op_adapt_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<adapt_huffman_decode_plan>& plan, std::shared_ptr<adapt_huffman_decode_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<adapt_huffman_decode_plan, adapt_huffman_decode_action, adapt_huffman_decode_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}

pArray exe_act_ind_adapt_huffman_encode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<adapt_huffman_encode_plan> myPlan;
	std::shared_ptr<adapt_huffman_encode_action> myAction;
	pQuery myQuery;
	get_op_adapt_huffman_encode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}
pArray exe_act_ind_adapt_huffman_decode(std::vector<pArray> sourceArr)
{
	std::shared_ptr<adapt_huffman_decode_plan> myPlan;
	std::shared_ptr<adapt_huffman_decode_action> myAction;
	pQuery myQuery;
	get_op_adapt_huffman_decode(sourceArr[0]->getDesc(), myPlan, myAction, myQuery);

	auto afterArray = myAction->execute(sourceArr, myQuery);

	return afterArray;
}

std::shared_ptr<adapt_huffman_encode_plan> getAdaptHuffmanEncodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<adapt_huffman_encode_plan, adapt_huffman_encode_array_pset>(sourceArrDesc, qry);
}
std::shared_ptr<adapt_huffman_decode_plan> getAdaptHuffmanDecodePlan(pArrayDesc sourceArrDesc, pQuery qry)
{
	return getSingleArrayParamPlan<adapt_huffman_decode_plan, adapt_huffman_decode_array_pset>(sourceArrDesc, qry);
}
}		// caDummy
}		// core
}		// msdb