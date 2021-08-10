#pragma once
#ifndef _MSDB_TEST_ACT_LZW_HUFFMAN_H_
#define _MSDB_TEST_ACT_LZW_HUFFMAN_H_

#include <pch.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_plan.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_action.h>
#include <op/lzw_huffman_decode/lzw_huffman_decode_plan.h>
#include <op/lzw_huffman_decode/lzw_huffman_decode_action.h>

namespace msdb
{
namespace caDummy
{
void get_op_lzw_huffman_encode(pArrayDesc sourceArrDesc, std::shared_ptr<lzw_huffman_encode_plan>& plan, std::shared_ptr<lzw_huffman_encode_action>& action, pQuery& qry);
void get_op_lzw_huffman_decode(pArrayDesc sourceArrDesc, std::shared_ptr<lzw_huffman_decode_plan>& plan, std::shared_ptr<lzw_huffman_decode_action>& action, pQuery& qry);

pArray exe_act_ind_lzw_huffman_encode(std::vector<pArray> sourceArr);
pArray exe_act_ind_lzw_huffman_decode(std::vector<pArray> sourceArr);

std::shared_ptr<lzw_huffman_encode_plan> getLzwHuffmanEncodePlan(pArrayDesc sourceArrDesc, pQuery qry);
std::shared_ptr<lzw_huffman_decode_plan> getLzwHuffmanDecodePlan(pArrayDesc sourceArrDesc, pQuery qry);
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_ACT_LZW_HUFFMAN_H_