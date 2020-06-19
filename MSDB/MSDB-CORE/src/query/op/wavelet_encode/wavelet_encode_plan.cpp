#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <util/math.h>
#include <cassert>

namespace msdb
{
wavelet_encode_plan::wavelet_encode_plan()
{
}
const char* msdb::wavelet_encode_plan::name()
{
	return "wavelet_encode";
}

void msdb::wavelet_encode_plan::initParamSets()
{
	this->addParamSet(pParamSet(new wavelet_encode_pset()));
}

//////////////////////////////
// pset
void msdb::wavelet_encode_pset::initParams()
{
	auto a = new opParamArrayPlaceholder();
	this->params_.push_back(_MSDB_MAKE_PARAM(opParamArrayPlaceholder));		// Source array
	this->params_.push_back(_MSDB_MAKE_PARAM(opParamConstPlaceholder));		// Target level
}

pArrayDesc msdb::wavelet_encode_pset::inferSchema()
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// Source array
	assert(this->params_[1]->type() == opParamType::CONST);		// Target level

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);
	eleDefault level;
	std::static_pointer_cast<opParamConst::paramType>(this->params_[1]->getParam())->getData(&level);
	
	size_t ratio = std::pow(2, level);
	
	for(dimensionId d = 0; d < aInferDesc->dimDescs_.size(); d++)
	{
		auto dDesc = aInferDesc->dimDescs_[d];
		dDesc->chunkSize_ = intDivCeil(dDesc->chunkSize_, ratio);
	}

	return aInferDesc;
}
}

