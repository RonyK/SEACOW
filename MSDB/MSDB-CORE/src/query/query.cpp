#include <query/query.h>

namespace msdb
{
timer& query::getTimer()
{
	return this->timer_;
}
}	// msdb