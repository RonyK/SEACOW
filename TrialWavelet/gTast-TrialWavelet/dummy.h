#pragma once
#include "pch.h"
#include <vector>

namespace caWavelet
{
	namespace caDummy
	{
		namespace data2D_s4x4
		{
#define dummyDataLength 16
			using dim_type = unsigned char;
			using value_type = char;

			//typedef unsigned char dim_type;
			//typedef char value_type;

			extern const size_t dimX;
			extern const size_t dimY;

			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;
			extern std::vector<dim_type> dims;
			extern size_t maxLevel;

			void getDummy(char* output, size_t length);
			void getWTDummy(char* output, size_t length);
			void getEXDummy(char* output, size_t length);
		}

		namespace data2D_s8x8
		{
#define dummyDataLength 64

			extern const size_t dimX;
			extern const size_t dimY;

			void getDummy(char* output, size_t length);
			void getWTDummy(char* output, size_t length);
			void getExDummy(char* output, size_t length);
		}
	}
}
