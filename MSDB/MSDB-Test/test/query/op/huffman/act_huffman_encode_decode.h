#pragma once
#ifndef _MSDB_TEST_OP_ACT_HUFFMAN_ENCODE_DECODE_H_
#define _MSDB_TEST_OP_ACT_HUFFMAN_ENCODE_DECODE_H_

#include <pch.h>
#include <op/huffman_encode/huffman_encode_plan.h>
#include <op/huffman_encode/huffman_encode_action.h>
#include <op/huffman_decode/huffman_decode_plan.h>
#include <op/huffman_decode/huffman_decode_action.h>

namespace msdb
{
namespace caDummy
{
void get_op_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<huffman_encode_plan>& plan, std::shared_ptr<huffman_encode_action>& action, pQuery& qry);
void get_op_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<huffman_decode_plan>& plan, std::shared_ptr<huffman_decode_action>& action, pQuery& qry);

pArray exe_act_ind_huffman_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_huffman_decode(std::vector<pArray> sourceArr);
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_OP_ACT_HUFFMAN_ENCODE_DECODE_H_