#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <string>
#include <vector>

namespace msdb
{
	class dimensionDesc;

	class dimensions
	{
		using container_type = std::vector<dimensionDesc>;

		container_type _dimensions;
	};

	class dimensionDesc
	{
		using dimension_type = unsigned int;

		dimension_type _start;
		dimension_type _end;
	};
}

#endif