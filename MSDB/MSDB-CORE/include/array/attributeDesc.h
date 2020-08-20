#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <array/attributeId.h>
#include <util/element.h>
#include <string>
#include <vector>
#include <memory>

namespace msdb
{
class attributeDesc;
using pAttributeDesc = std::shared_ptr<attributeDesc>;

class attributeDescs;
using pAttributeDescs = std::shared_ptr<attributeDescs>;

class attributeDescs : public std::vector<pAttributeDesc>, public std::enable_shared_from_this<attributeDescs>
{
public:
	attributeDescs();
	attributeDescs(const attributeDescs& mit);
};

class attributeDesc
{
public:
	attributeDesc(attributeId id, std::string name, eleType type);

public:
	attributeId id_;
	std::string name_;
	eleType type_;
	size_t typeSize_;
};
}

#endif