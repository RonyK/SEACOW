#pragma once
#ifndef _MSDB_SPIHTDUMMY_H_
#define _MSDB_SPIHTDUMMY_H_

#include <pch.h>
#include <vector>

namespace msdb
{
	namespace caDummy
	{
		using dim_type = position_t;
		// signed char
		namespace data2D_spiht_sc4x4
		{
			using value_type = char;

			static const size_t dataLength = 16;
			static const size_t dimX = 4;
			static const size_t dimY = 4;
			static const size_t maxLevel = 0;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;

			void getWTDummy(value_type* output, size_t length);
		}
	}
}

#endif	//_MSDB_SPIHTDUMMY_H_

