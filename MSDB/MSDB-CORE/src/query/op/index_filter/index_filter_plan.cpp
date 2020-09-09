#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

namespace msdb
{
index_filter_plan::index_filter_plan()
{
}

const char* index_filter_plan::name()
{
	return "index_filter";
}

pAction index_filter_plan::makeAction()
{
	return std::make_shared<index_filter_action>();
}

index_filter_array_pset::index_filter_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::PREDICATE);
}

pArrayDesc index_filter_array_pset::inferSchema()
{
	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
pBitmapTree index_filter_array_pset::inferBottomUpBitmap()
{
	pArrayDesc arrDesc = this->inferSchema();

	// TODO::remove a const attribute id and use scan for all attributes
	for (auto attrDesc : *arrDesc->getAttrDescs())
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			return this->inferBottomUpAttrBitmap<char>(arrDesc, attrDesc);
			break;
		case eleType::INT8:
			return this->inferBottomUpAttrBitmap<int8_t>(arrDesc, attrDesc);
			break;
		case eleType::INT16:
			return this->inferBottomUpAttrBitmap<int16_t>(arrDesc, attrDesc);
			break;
		case eleType::INT32:
			return this->inferBottomUpAttrBitmap<int32_t>(arrDesc, attrDesc);
			break;
		case eleType::INT64:
			return this->inferBottomUpAttrBitmap<int64_t>(arrDesc, attrDesc);
			break;
		case eleType::UINT8:
			return this->inferBottomUpAttrBitmap<uint8_t>(arrDesc, attrDesc);
			break;
		case eleType::UINT16:
			return this->inferBottomUpAttrBitmap<uint16_t>(arrDesc, attrDesc);
			break;
		case eleType::UINT32:
			return this->inferBottomUpAttrBitmap<uint32_t>(arrDesc, attrDesc);
			break;
		case eleType::UINT64:
			return this->inferBottomUpAttrBitmap<uint64_t>(arrDesc, attrDesc);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
}
}	// msdb