#include <stdafx.h>
#include <query/query.h>

namespace msdb
{
query::query()
{
	this->timer_ = std::make_shared<timer>();
}
pTimer query::getTimer()
{
	return this->timer_;
}
}	// msdb