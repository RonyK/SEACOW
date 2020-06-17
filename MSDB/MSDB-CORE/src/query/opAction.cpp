#include <query/opAction.h>

namespace msdb
{
const pArrayDesc msdb::opAction::getArrayDesc()
{
	return this->aDesc_;
}
void opAction::setParams(const parameters& params)
{
	this->params_ = params;
}
}
