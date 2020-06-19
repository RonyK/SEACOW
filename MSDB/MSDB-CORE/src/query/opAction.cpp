#include <query/opAction.h>

namespace msdb
{
opAction::opAction()
{
}

opAction::~opAction()
{
}

const pArrayDesc msdb::opAction::getArrayDesc()
{
	return this->aDesc_;
}

void opAction::setParams(const parameters& params)
{
	this->params_ = params;
}

void opAction::setArrayDesc(pArrayDesc aDesc)
{
	this->aDesc_ = aDesc;
}
}
