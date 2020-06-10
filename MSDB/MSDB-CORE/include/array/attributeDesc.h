#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <array/attributeId.h>
#include <string>
#include <vector>

namespace msdb
{
	class attributeDesc;

	class attributes : public std::vector<attributeDesc>
	{

	};

	class attributeDesc
	{
	public:
		attributeId _id;
		std::string _name;
		// _type
	};
}

#endif