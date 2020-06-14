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
		this->parameters_.push_back((*it));
	}
}

bool opParamSet::isMatch(parameters pSet)
{
	if (this->parameters_.size() != pSet.size())
		return false;

	auto it = this->parameters_.begin();
	auto iit = pSet.begin();
	for(; it != this->parameters_.end(); it++, iit++)
	{
		
	}
}
}