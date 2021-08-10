#pragma once
#ifndef _MSDB_TEST_OP_ACT_ADAPT_HUFFMAN_ENCODE_DECODE_H_
#define _MSDB_TEST_OP_ACT_ADAPT_HUFFMAN_ENCODE_DECODE_H_

#include <pch.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_plan.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_action.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_plan.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_action.h>
#include <dummy/opPlanDummy.h>

namespace msdb
{
namespace caDummy
{
void get_op_adapt_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<adapt_huffman_encode_plan>& plan, std::shared_ptr<adapt_huffman_encode_action>& action, pQuery& qry);
void get_op_adapt_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<adapt_huffman_decode_plan>& plan, std::shared_ptr<adapt_huffman_decode_action>& action, pQuery& qry);

pArray exe_act_ind_adapt_huffman_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_adapt_huffman_decode(std::vector<pArray> sourceArr);

std::shared_ptr<adapt_huffman_encode_plan> getAdaptHuffmanEncodePlan(pArrayDesc sourceArrDesc, pQuery qry);
std::shared_ptr<adapt_huffman_decode_plan> getAdaptHuffmanDecodePlan(pArrayDesc sourceArrDesc, pQuery qry);
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_OP_ACT_ADAPT_HUFFMAN_ENCODE_DECODE_H_