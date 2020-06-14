#include <query/opAction.h>

namespace msdb
{
const arrayDesc& msdb::opAction::getSchema()
{
	return this->schema_;
}
void opAction::setParams(const parameters& params)
{
	this->params_ = params;
}
}
