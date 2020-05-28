#pragma once
#ifndef _MSDB_ARRAYDESC_H_
#define _MSDB_ARRAYDESC_H_

#include <string>
#include <vector>
#include "attributeDesc.h"
#include "dimensionDesc.h"

namespace msdb
{
	class arrayDesc
	{
		using arrayId = unsigned int;

	private:
		arrayId aid;
		std::string arrayName;

		attributes attrs;
		dimensions dims;
	};
}

#endif