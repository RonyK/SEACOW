#pragma once
#ifndef _MSDB_DUMMY_H_
#define _MSDB_DUMMY_H_

#include <pch.h>
#include <vector>


namespace msdb
{
	namespace caDummy
	{
		// signed char
		namespace data2D_sc4x4
		{
			using dim_type = unsigned char;
			using value_type = char;

			static const size_t dataLength = 16;
			static const size_t dimX = 4;
			static const size_t dimY = 4;
			static const size_t maxLevel = 0;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;

			void getDummy(value_type* output, size_t length);
			void getWTDummy(value_type* output, size_t length);
			void getExDummy(value_type* output, size_t length);
#undef dummyDataLength
		}

		// signed char
		namespace data2D_sc8x8
		{
			using dim_type = unsigned char;
			using value_type = char;

			static const size_t dataLength = 64;
			static const size_t dimX = 8;
			static const size_t dimY = 8;
			static const size_t maxLevel = 2;

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
			using dim_type = unsigned int;
			using value_type = int;

			static const size_t dataLength = 64;
			static const size_t dimX = 8;
			static const size_t dimY = 8;
			static const size_t maxLevel = 2;

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

#endif	//_MSDB_DUMMY_H_