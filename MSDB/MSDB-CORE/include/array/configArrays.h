#pragma once
#ifndef _MSDB_CONFIGARRAYS_H_
#define _MSDB_CONFIGARRAYS_H_

#include <array/arrayDesc.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <system/config.h>
#include <xml/tinyxml2.h>
#include <vector>

// Meta data for arrays is recorded in XML file at the config folder.

//	Array name
//	DimensionDesc
//		Dimension size
//		Dimensions
//			Dimension
//				Dimension name
//				Dimension type
//				Start
//				End
//				Chunk size
//	AttributeDesc
//		Attribute size
//		Attributes
//			Attribute
//				Attribute name
//				Attribute type

namespace msdb
{
#define _MSDB_XML_ARRAYDESC			"array_desc"
#define _MSDB_XML_ARRAYS			"arrays"

#define _MSDB_XML_DIMENSIONDESC		"dimension_desc"
#define _MSDB_XML_DIMENSIONS		"dimensions"

#define _MSDB_XML_ATTRIBUTEDESC		"attribute_desc"
#define _MSDB_XML_ATTRIBUTES		"attributes"

using arrayDescList = std::vector<arrayDesc>;

namespace configjobs
{
// config array
void getAttributeDescList(tinyxml2::XMLNode* root, void* list);
//void setAttributeDescList(attributes* list);

void getDimensionDescList(tinyxml2::XMLNode* root, void* list);
//void setDimensionDescList(dimensions* list);

// configArrayList
void getArrayDesc(tinyxml2::XMLNode* root, void* list);
}

class configDiemnsion : public config
{
public:
	configDiemnsion(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getDimensionDesc(dimensionDesc* desc);
	void setDimensionDesc(dimensionDesc* desc);
};

class configAttribute : public config
{
public:
	configAttribute(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getAttributeDesc(attributeDesc* desc);
	void setAttributeDesc(attributeDesc* desc);
};


class configArray : public config
{
public:
	configArray(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();
	
	void getArrayDesc(arrayDesc* desc);
	void setArrayDesc(arrayDesc* desc);
};

class configArrayList : public config
{
public:
	configArrayList(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getArrayDescList(arrayDescList* list);
	void setArrayDescList(arrayDescList* list);
};
}
#endif		// _MSDB_CONFIGARRAYS_H_
