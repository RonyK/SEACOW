#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <util/coordinate.h>
#include <array/dimensionId.h>
#include <string>
#include <vector>

namespace msdb
{
	class dimensionDesc;

	class dimensions : public std::vector<dimensionDesc>
	{
	public:
		std::vector<position_t> getDims();
	};

	class dimensionDesc
	{
	public:
		position_t getLength();

	public:
		using dimension_type = position_t;

		dimensionId id_;
		std::string name_;
		dimension_type start_;
		dimension_type end_;
	};
}

#endif