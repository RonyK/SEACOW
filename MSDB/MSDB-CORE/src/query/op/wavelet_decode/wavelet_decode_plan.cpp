#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>
#include <cassert>

namespace msdb
{
wavelet_decode_plan::wavelet_decode_plan()
{
}
const char* wavelet_decode_plan::name()
{
	return "wavelet_decode";
}
pAction wavelet_decode_plan::getAction()
{
	auto wdAction = std::make_shared<wavelet_decode_action>();
	return wdAction;
}

wavelet_decode_pset::wavelet_decode_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
}

pArrayDesc wavelet_decode_pset::inferSchema()
{
	assert(this->params_[0]->type() == opParamType::ARRAY);
	assert(this->params_[1]->type() == opParamType::CONST);		// Target level
	//assert(this->params_[2]->type() == opParamType::INTLIST);	// Target dimSchema

	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(
		this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(
		*aSourceDesc);
	eleDefault level;
	//std::static_pointer_cast<opParamConst::paramType>(
	//	this->params_[1]->getParam())->getData(&level);

	//// Reset chunkSize
	//auto chunkSizes = std::static_pointer_cast<opParamIntList::paramType>(
	//	this->params_[2]->getParam());
	//for (dimensionId d = 0; d < aInferDesc->dimDescs_->size(); ++d)
	//{
	//	auto dDesc = aInferDesc->dimDescs_[d];
	//	dDesc->chunkSize_ = chunkSizes->at(d);
	//}

	return aInferDesc;
}
}	// msdb