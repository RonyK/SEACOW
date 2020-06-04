#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <array/attributeId.h>
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
		attributeId _id;
		std::string _name;
		// type
	};
}

#endif