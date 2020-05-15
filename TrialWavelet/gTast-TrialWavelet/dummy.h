#pragma once
#include "pch.h"
#include <vector>

namespace caWavelet
{
	namespace caDummy
	{
		// signed char
		namespace data2D_sc4x4
		{
#define dummyDataLength 16
			using dim_type = unsigned char;
			using value_type = char;

			extern const size_t dimX;
			extern const size_t dimY;
			extern size_t maxLevel;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;
			

			void getDummy(value_type* output, size_t length);
			void getWTDummy(value_type* output, size_t length);
			void getExDummy(value_type* output, size_t length);
		}

		// signed char
		namespace data2D_sc8x8
		{
#define dummyDataLength 64
			using dim_type = unsigned char;
			using value_type = char;

			extern const size_t dimX;
			extern const size_t dimY;
			extern const size_t maxLevel;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;
			

			void getDummy(value_type* output, size_t length);
			void getWTDummy(value_type* output, size_t length);
			void getExDummy(value_type* output, size_t length);
			void getExMMT(value_type* output, size_t length);
		}

		// signed integer
		namespace data2D_si8x8
		{
#define dummyDataLength 64
			using dim_type = unsigned char;
			using value_type = int;

			extern const size_t dimX;
			extern const size_t dimY;
			extern const size_t maxLevel;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;


			void getDummy(char* output, size_t length);
			void getWTDummy(char* output, size_t length);
			void getExDummy(char* output, size_t length);
			void getExMMT(char* output, size_t length);
		}
	}
}
