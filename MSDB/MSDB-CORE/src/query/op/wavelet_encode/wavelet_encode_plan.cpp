#include <op/wavelet_encode/wavelet_encode_plan.h>
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
	this->parameters_.push_back(_MSDB_MAKE_PARAM(opParamArrayPlaceholder));
}

arrayDesc msdb::wavelet_encode_pset::inferSchema()
{
	assert(this->parameters_.size() == 1);
	assert(this->parameters_[0]->type() == opParamType::ARRAY);

	//std::shared_ptr<opParamArray> pA = std::dynamic_pointer_cast<opParamArray>(this->parameters_[0]);
	arrayDesc aDesc;
	this->parameters_[0]->getParam(&aDesc);
	return aDesc;
}
}

