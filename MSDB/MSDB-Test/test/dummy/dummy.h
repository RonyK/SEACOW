#pragma once
#ifndef _MSDB_DUMMY_H_
#define _MSDB_DUMMY_H_

#include <pch.h>
#include <array/array.h>
#include <vector>

namespace msdb
{
	namespace caDummy
	{
		using dim_type = position_t;
		// signed char
		namespace data2D_sc4x4
		{
			using value_type = char;

			static const size_t dataLength = 16;
			static const size_t dimX = 4;
			static const size_t dimY = 4;
			static const size_t maxLevel = 0;
			static const arrayId aid = 441;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;

			void getDummy(value_type* output, size_t length);
			void getChunkDummy(value_type* output, size_t length);
			void getWTDummy(value_type* output, size_t length);
			void getWTChunkDummy(value_type* output, size_t length);
			void getExDummy(value_type* output, size_t length);
			void getExMMTBuilded(value_type(*minOutput)[2][2], value_type(*maxOutput)[2][2], size_t length);
			std::vector<pArray> getSourceArray();
		}

		// signed char
		namespace data2D_sc8x8
		{
			using value_type = char;

			static const size_t dataLength = 64;
			static const size_t dimX = 8;
			static const size_t dimY = 8;
			static const size_t maxLevel = 2;
			static const arrayId aid = 881;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;


			void getDummy(value_type* output, size_t length);
			void getWTDummy(value_type* output, size_t length);
			void getExDummy(value_type* output, size_t length);
			void getExMMTBuilded(value_type(*minOutput)[4][4], value_type(*maxOutput)[4][4], size_t length);
			void getExSerialMMT(value_type* output, size_t length);
			std::vector<pArray> getSourceArray();
		}

		// signed integer
		namespace data2D_si8x8
		{
			using value_type = int;

			static const size_t dataLength = 64;
			static const size_t dimX = 8;
			static const size_t dimY = 8;
			static const size_t maxLevel = 2;
			static const arrayId aid = 882;

			extern std::vector<dim_type> dims;
			extern std::vector<dim_type> chunkDims;
			extern std::vector<dim_type> chunkNums;

			void getDummy(char* output, size_t length);
			void getWTDummy(char* output, size_t length);
			void getExDummy(char* output, size_t length);
			void getExMMTBuilded(char* output, size_t length);
		}
	}
}

#endif	//_MSDB_DUMMY_H_