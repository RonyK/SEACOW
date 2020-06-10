#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <array/dimensionId.h>
#include <string>
#include <vector>

namespace msdb
{
	class dimensionDesc;

	class dimensions : public std::vector<dimensionDesc>
	{

	};

	class dimensionDesc
	{
	public:
		using dimension_type = unsigned int;

		dimensionId _id;
		std::string _name;
		dimension_type _start;
		dimension_type _end;
	};
}

#endif