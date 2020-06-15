#pragma once
#ifndef _MSDB_ARRAYDESC_H_
#define _MSDB_ARRAYDESC_H_

#include <string>
#include <vector>
#include <array/arrayId.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <memory>

namespace msdb
{
	class arrayDesc
	{
	public:
		arrayDesc();
		arrayDesc(arrayId aid, std::string arrayName, attributes attrs, dimensions dims);
		~arrayDesc();

	public:
		arrayId id_;
		std::string name_;

		attributes attrs_;
		dimensions dims_;
	};
}

#endif