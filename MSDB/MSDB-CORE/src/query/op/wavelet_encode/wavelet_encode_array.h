#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
#define _MSDB_OP_WAVELET_ENCODE_ARRAY_H_

#include <array/array.h>

namespace msdb
{
class wavelet_encode_array : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	wavelet_encode_array(pArrayDesc desc, const size_t maxLevel, const dimension originalChunkSize);

	/*
	 * In wavelet_encode_array,
	 * number of chunks are multipled by (2)^(level)^(num of dims).
	 *
	 * For example, there is 2-dimensional array.
	 *
	 * [Level 0]
	 * In level 0, number of chunks is sustained.
	 * ┏━━━┓	┏━┳━┓	┏━━━┓
	 * ┃ 0 ┃ →	┣━╋━┫ →	┃ 0 ┃
	 * ┗━━━┛	┗━┻━┛	┗━━━┛
	 *
	 * [Level 1]
	 * In level 1, number of chunks increased into 4.
	 * ┏━━━━━━━┓	┏━┳━┳━┯━┓	┏━━━┳━━━┓
	 * ┃       ┃	┣━╋━╉─┼─┨	┃ 0 ┃ 1 ┃
	 * ┃   0   ┃ →	┣━╇━╋━┿━┫ →	┣━━━╋━━━┫
	 * ┃       ┃	┠─┼─╂─┼─┨	┃ 2 ┃ 3 ┃
	 * ┗━━━━━━━┛	┗━┷━┻━┷━┛	┗━━━┻━━━┛
	 *
	 * Example 01>
	 * weChunks for (0,0) chunk
	 * ┏━━━┳━━━┓	┏━┳━┳━┑
	 * ┃ * ┃   ┃	┣━╋━╉─┘
	 * ┣━━━╋━━━┫ →	┣━╇━╋━┑
	 * ┃   ┃   ┃	┖─┘ ┖─┘
	 * ┗━━━┻━━━┛
	 *
	 * Example 02>
	 * weChunks for (0,1) chunk
	 * ┏━━━┳━━━┓	┏━┳━┓ ┍━┓
	 * ┃   ┃ * ┃	┣━╋━┫ └─┚
	 * ┣━━━╋━━━┫ →	┗━╇━┫ ┍━┓
	 * ┃   ┃   ┃	  └─┚ └─┚
	 * ┗━━━┻━━━┛
	 */

public:
	size_t getMaxLevel();
	dimension getOrigianlChunkDims();

private:
	bool isMaxLevelAvailable(size_t maxLevel);

protected:
	size_t maxLevel_;
	dimension originalChunkDims_;
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
