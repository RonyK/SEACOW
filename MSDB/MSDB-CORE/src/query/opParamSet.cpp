#include <query/opParamSet.h>

namespace msdb
{
opParamSet::opParamSet()
{
	this->initParams();
}
opParamSet::opParamSet(parameters pSet)
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
}