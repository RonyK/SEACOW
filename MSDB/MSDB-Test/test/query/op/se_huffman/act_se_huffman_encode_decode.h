#pragma once
#ifndef _MSDB_TEST_OP_ACT_SE_HUFFMAN_ENCODE_DECODE_H_
#define _MSDB_TEST_OP_ACT_SE_HUFFMAN_ENCODE_DECODE_H_

#include <pch_test.h>
#include <op/se_huffman_encode/se_huffman_encode_plan.h>
#include <op/se_huffman_encode/se_huffman_encode_action.h>
#include <op/se_huffman_decode/se_huffman_decode_plan.h>
#include <op/se_huffman_decode/se_huffman_decode_action.h>
#include <dummy/opPlanDummy.h>

namespace msdb
{
namespace core
{
namespace caDummy
{
void get_op_se_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<se_huffman_encode_plan>& plan, std::shared_ptr<se_huffman_encode_action>& action, pQuery& qry);
void get_op_se_huffman_decode(pArrayDesc sourceArrDesc, eleDefault level, std::shared_ptr<se_huffman_decode_plan>& plan, std::shared_ptr<se_huffman_decode_action>& action, pQuery& qry);

pArray exe_act_ind_se_huffman_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_se_huffman_decode(std::vector<pArray> sourceArr, eleDefault wtLevel);

std::shared_ptr<se_huffman_encode_plan> getSeHuffmanEncodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry);
std::shared_ptr<se_huffman_decode_plan> getSeHuffmanDecodePlan(pArrayDesc sourceArrDesc, eleDefault wtLevel, pQuery qry);
}		// caDummy
}		// core
}		// msdb
#endif	// _MSDB_TEST_OP_ACT_SE_HUFFMAN_ENCODE_DECODE_H_