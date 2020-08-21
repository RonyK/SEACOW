#include <query/opParamSet.h>

namespace msdb
{
opParamSet::opParamSet()
{
	//this->initParams();
}
opParamSet::opParamSet(parameters& pSet)
{
	for(auto it = pSet.begin(); it != pSet.end(); it++)
	{
		this->params_.push_back((*it));
	}
}

bool opParamSet::isMatch(parameters pSet)
{
	if (this->params_.size() != pSet.size())
		return false;

	auto it = this->params_.begin();
	auto iit = pSet.begin();
	for(; it != this->params_.end(); it++, iit++)
	{
		
	}
}
parameters opParamSet::getParam()
{
	return this->params_;
}
opArrayParamSet::opArrayParamSet(parameters& pSet)
	: opParamSet(pSet)
{
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array
}
pArrayDesc opArrayParamSet::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmap opArrayParamSet::inferBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();

	return std::make_shared<bitmap>(chunkSpace.area(), true);
}
}