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
class arrayDesc;

using pArrayDesc = std::shared_ptr<arrayDesc>;

class arrayDesc : public std::enable_shared_from_this<arrayDesc>
{
public:
	arrayDesc();
	arrayDesc(const arrayId aid, const std::string arrayName, 
			  const dimensionDescs& dimDescs, const attributeDescs& attrDescs);
	arrayDesc(const arrayDesc& mit);
	~arrayDesc();

public:
	dimensionDescs& getDimDescs();
	attributeDescs& getAttrDescs();

public:
	arrayId id_;
	std::string name_;

	dimensionDescs dimDescs_;
	attributeDescs attrDescs_;
};
}

#endif