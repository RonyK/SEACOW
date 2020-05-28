#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <string>
#include <vector>

namespace msdb
{
	class attributeDesc;

	class attributes
	{
		using container_type = std::vector<attributeDesc>;

		container_type _attributes;
	};

	class attributeDesc
	{
		std::string _name;
		// type
	};
}

#endif